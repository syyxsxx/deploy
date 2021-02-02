// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
    //parser yaml file
    Deploy::ConfigParser parser;
    parser.Load(FLAGS_cfg_file);

    // data preprocess
    // preprocess init
    Deploy::DetPreprocess detpreprocess;
    detpreprocess.Init(parser)
    // postprocess init
    Deploy::DetPreprocess detpostprocess;
    detpostprocess.Init(parser)
    if (FLAGS_image_list != "") {
        //img_list
    }
    else {
        //read image
        std::vector<cv::mat> imgs
        cv::mat img;
        img = cv::imread(FLAGS_cfg_image);
        imgs.push_back(std::move(img))
        //create inpus and shape_traces
        std::vector<Deploy::ShapeInfo> shape_traces;
        std::vector<Deploy::DataBlob> inputs;
        //preprocess 
        detpreprocess.Run(imgs, &inputs, &shape_traces);
        //infer
        std::vector<Deploy::Datablob> outputs;
        Deploy::PpInferenceEngine ppi_engine;
        Deploy::PpiConfig ppi_config;
        ppi_engine.Init(FLAGS_model_dir, ppi_config);
        ppi_engine.Infer(inputs, &outputs);

        //postprocess
        
        std::vector<Deploy::DetResult> detresults;
        detpostprocess.Run(detresults, shape_traces)
    }
}
