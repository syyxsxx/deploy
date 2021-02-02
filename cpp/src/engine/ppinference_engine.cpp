



#include "include/deploy/engine/ppinference_engine.h"

namespace Deploy {

void PpInferenceEngine::Init(std::string model_dir, PPI_config &engine_config) {
    paddle::AnalysisConfig config;
    std::string prog_file = model_dir + OS_PATH_SEP + "__model__";
    std::string params_file = model_dir + OS_PATH_SEP + "__params__";
    if (engine_config.use_mkl && !engine_config.use_gpu) {
        config.EnableMKLDNN();
        config.SetCpuMathLibraryNumThreads(engine_config.mkl_thread_num);
    }
    if (engine_config.use_gpu) {
        config.EnableUseGpu(100, engine_config.gpu_id);
    } else {
        config.DisableGpu();
    }
    config.SwitchUseFeedFetchOps(false);
    config.SwitchSpecifyInputNames(true);
    #if defined(__arm__) || defined(__aarch64__)
        config.SwitchIrOptim(false);
    #else
        config.SwitchIrOptim(engine_config.use_ir_optim);
    #endif
    config.EnableMemoryOptim();
    if (use_trt && use_gpu) {
        if (engine_config.precision == 0) {
            paddle::AnalysisConfig::Precision precision = paddle::AnalysisConfig::Precision::kFloat32;
        }
        else if(engine_config.precision == 1) {
            paddle::AnalysisConfig::Precision precision = paddle::AnalysisConfig::Precision::kHalf;
        }
        else if(engine_config.precision == 2) {
            paddle::AnalysisConfig::Precision precision = paddle::AnalysisConfig::Precision::Int8;
        }
        else {
            std::cerr << "Can not support the set precision, pealse set engine_config.precision == 0,1,2" << std::endl;
        }
        
        config.EnableTensorRtEngine(
            1 << 10 /* workspace_size*/,
            engine_config.batch_size /* max_batch_size*/,
            engine_config.min_subgraph_size /* min_subgraph_size*/,
            precision /* precision*/,
            engine_config.use_static /* use_static*/,
            engine_config.use_calib_mode /* use_calib_mode*/);
    }
    predictor_ = std::move(CreatePaddlePredictor(config));
}

void PpInferenceEngine::Infer(std::vector<std::DateBlob> &inputs, std::vector<DateBlob> *outputs) {
    for (int i = 0; i < inputs.size(); i++) {
        auto in_tensor = predictor_->GetInputTensor(inputs[i].name);
        in_tensor->Reshape(inputs[i].shape);
        if (inputs[i].dtype == 0) {
            in_tensor->copy_from_cpu((float*)inputs[i].data);
        }
        else if (inputs[i].dtype == 1) {
            in_tensor->copy_from_cpu((int64_t*)inputs[i].data);
        }
        else if (inputs[i].dtype == 2) {
            in_tensor->copy_from_cpu((int*)inputs[i].data);
        }
        else if (inputs[i].dtype == 3) {
            in_tensor->copy_from_cpu((uint8_t*)inputs[i].data);
        }
    }
    //predict
    predictor_->ZeroCopyRun();

    //output
    auto output_names = predictor_->GetOutputNames();
    for (output_name : output_names){
        auto output_tensor = predictor_->GetOutputTensor(output_name);
        auto output_tensor_shape = output_tensor->shape();
        DateBlob output;
        output.name = output_name;
        output.shape.assign(output_tensor_shape.begin(), output_tensor_shape.end());
        output_tensor->copy_to_cpu(output.data());
        output.dtype = paddle::DataType(output_tensor->type());
        output.lod = output_tensor->lod();
        outputs->push_back(std::move(output));
    }
    

}

    
}