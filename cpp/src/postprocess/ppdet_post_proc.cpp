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

#include "include/deploy/postprocess/ppdet_post_proc.h"

namespace Deploy {

void PpDetPostProc::Init(const ConfigParser &parser) {
    model_arch_ = parser.Get<std::string>("model_name");
    labels_.clear();
    for (const auto &item : parser.Get<std::string>("labels") ) {
        int index = labels.size();
        labels[index] = item.as<std::string>();
    }
}

bool PpDetPostProc::Run(const std::vector<DataBlob> &outputs, std::vector<ShapeInfo> &shape_traces, std::vector<PpDetResult> *det_results) {
    det_results->clear();
    DataBlob output_blob = outputs[0];
    float *output_data = (float*)output_blob.data;
    auto lod_data = output_blob.lod;
    int batchsize = shape_traces.size();
    //box postprocess
    det_results->resize(batchsize)
    for (int i = 0; i < lod_vector[0].size() - 1; ++i) {
        int rh = 1;
        int rw = 1;
        if (model_arch_ == "SSD" || model_arch_ == "Face") {
            rh =  shape_traces[i].shape[0][1];
            rw =  shape_traces[i].shape[0][0];
        }
        for (int j = lod_vector[0][i]; j < lod_vector[0][i + 1]; ++j) {
            Box box;
            box.category_id = static_cast<int>(round(output_box[j * 6]));
            box.category = labels[box.category_id];
            box.score = output_data[1 + j * 6];
            int xmin = (output_data[2 + j * 6] * rw);
            int ymin = (output_data[3 + j * 6] * rh);
            int xmax = (output_data[4 + j * 6] * rw);
            int ymax = (output_data[5 + j * 6] * rh);
            int wd = xmax - xmin;
            int hd = ymax - ymin;
            box.coordinate = {xmin, ymin, wd, hd};
            results->boxes.push_back(std::move(box));
    }

}

}//namespace