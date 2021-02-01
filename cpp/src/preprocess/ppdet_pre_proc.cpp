  
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

#pragma once

#include "ppdet_pre_proc.h"


namespace Deploy {

bool DetProecess::Init(const ConfigParser &parser) {
    BuildTransform(parser);
    model_arch_ = parser.Get<std::string>("model_name")
}

bool DetProecess::Run(const std::vector<cv::Mat> &imgs, std::vector<std::vector<DataBlob>> *inputs, std::vector<ShapeInfo> *shape_traces){
    inputs.clear();
    for (int i=0; i < imgs.size(); i++) {
        ShapeInfo im_shape;
        cv::Mat im = imgs[i].clone();
        //(w,h)
        std::vector<int> size = {im.cols, im.rows};
        im_shape.transform_order.push_back("Origin")
        im_shape.shape["Origin"] = size;
        for (int j=0; j < transforms.size(); j++) {
            if (!transforms[j]->Run(&im)) {
                std::cerr << "Apply transforms to image failed!" << std::endl;
                return false;
            }
            if (!transforms[j]->Shape_infer(&im_shape)) {
                std::cerr << "Apply shape inference failed!" << std::endl;
                return false;
            }
        }
        shape_traces->push_back(im_shape);
        std::vector<DataBlob> input;
        // img data for input
        DataBlob img_blob;
        img_blob.name = "image";
        std::vector<int> input_shape = im_shape.shape[im_shape.tranform_order.back()];
        img_blob.shape.assign(input_shape.begin(), input_shape.end());
        img_blob.dtype = 1;
        int input_size = 1;
        for (const auto& i : input_shape) {
            input_size *= i;
        }
        memrcy(img_blob.data, im.data, input_size * sizeof(float));
        // Additional information for input
        

    }
}

}