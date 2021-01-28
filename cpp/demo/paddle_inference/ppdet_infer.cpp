#include <glog/logging.h>

#include "yaml-cpp/yaml.h"
#include "include/deploy/config.h"
#include "include/deploy/blob.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


DEFINE_string(model_dir, "", "Path of inference model");
DEFINE_string(cfg_file, "", "Path of yaml file");
DEFINE_string(pp_type, "", "Type of Paddle toolkit");
DEFINE_string(image, "", "Path of test image file");

int main(int argc, char** argv) {
    // Parsing command-line
    google::ParseCommandLineFlags(&argc, &argv, true);
    //paser yaml file
    Deploy::ConfigPaser paser;
    paser.Load(FLAGS_cfg_file);

    // data preprocess
    // preprocess init
    Deploy::DetPreprocess detpreprocess;
    detpreprocess.Init(config)
    if (FLAGS_image_list != "") {
        //img_list
    }
    else {
        //read image
        std::vector<cv::mat> imgs
        cv::mat img;
        img = cv::imread(FLAGS_cfg_image);
        imgs.push_back(img)
        //create inpus and shape_traces
        std::vector<Deploy::ShapeInfo> shape_traces;
        std::vector<std::vector<Deploy::DataBlob>> inputs;
        //preprocess 
        detpreprocess.Run(imgs, &inputs, &shape_traces);
        //infer
        std::vector<std::vector<Deploy::Datablob>> outputs;
        Deploy::PpInferenceEngine ppi_engine;
        Deploy::PpiConfig ppi_config;
        ppi_engine.Init(FLAGS_model_dir, ppi_config);
        ppi_engine.Infer(inputs, &outputs);

        //postprocess
        Deploy::DetPreprocess detpostprocess;
        detpostprocess.Init(config)
        std::vector<Deploy::DetResult> detresults;
        detpostprocess.Run(detresults, shape_traces)
    }
}
