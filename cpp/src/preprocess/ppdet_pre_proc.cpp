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


#include "include/deploy/preprocess/ppdet_pre_proc.h"


namespace Deploy {

bool PaddleDetPreProc::Init(const ConfigParser &parser) {
    BuildTransform(parser);
    model_arch_ = parser.Get<std::string>("model_name");
}

bool PaddleDetPreProc::Run(const std::vector<cv::Mat> &imgs, std::vector<DataBlob> *inputs, std::vector<ShapeInfo> *shape_traces) {
    inputs->clear();
    shape_traces->clear();
    int batchsize = imgs.size();
    shape_traces->resize(batchsize);
    DataBlob img_blob;
    DataBlob im_size_blob;
    ShapeInfer(imgs, shape_traces);
    std::vector<int> max_shape = GetMaxSize();
    std::vector<cv::Mat> image;
    image.assign(imgs.begin(), imgs.end());
    if (!RunTransform(&image)) {
        std::cerr << "Apply transforms to image failed!" << std::endl;
        return false;
    }
    for (int i=0; i < batchsize; i++) {
        // img data for input
        std::vector<int> origin_size = {(*shape_traces)[i].shape[0][1], (*shape_traces)[i].shape[0][0]};
        int input_size = max_shape[0] * max_shape[1] * 3;
        memcpy(img_blob.data + i * input_size * sizeof(float) , image[i].data, input_size * sizeof(float));
        // Additional information for input
        if (model_arch_ == "YOLO") {
            memcpy(im_size_blob.data + i * 2 * sizeof(int), origin_size.data(), 2 * sizeof(int));
        }
    }
    img_blob.shape = {batchsize, 3, max_shape[1], max_shape[0]};
    img_blob.dtype = 0;
    img_blob.name = "image";
    inputs->push_back(std::move(img_blob));
    if (model_arch_ == "YOLO") {
        im_size_blob.name = "im_size";
        im_size_blob.shape = {batchsize, 2};
        im_size_blob.dtype = 2;
        inputs->push_back(std::move(im_size_blob));
    }
}

}