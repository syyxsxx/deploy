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

#include "preprocess.h"

namespace Deploy {   

bool BasePreprocess::BuildTransform(const ConfigParser &parser) {
    transforms.clear();
    transforms_node = parser.GetTransforms();
    for (const auto& node : transforms_node) {
        std::string name = node.begin()->first.as<std::string>();
        std::shared_ptr<Transform> transform = CreateTransform(name);
        transform->Init(node.begin()->second);
        transforms.push_back(transform);
    }
}

std::shared_ptr<Transform> BasePreprocess::CreateTransform(
    const std::string& transform_name) {
  if (transform_name == "Normalize") {
    return std::make_shared<Normalize>();
  } else if (transform_name == "ResizeByShort") {
    return std::make_shared<ResizeByShort>();
  } else if (transform_name == "ResizeByLong") {
    return std::make_shared<ResizeByLong>();
  } else if (transform_name == "CenterCrop") {
    return std::make_shared<CenterCrop>();
  } else if (transform_name == "Permute") {
    return std::make_shared<Permute>();
  } else if (transform_name == "Resize") {
    return std::make_shared<Resize>();
  } else if (transform_name == "Padding") {
    return std::make_shared<Padding>();
  } else if (transform_name == "Clip") {
    return std::make_shared<Clip>();
  } else if (transform_name == "RGB2BGR") {
    return std::make_shared<RGB2BGR>();
  } else if (transform_name == "BGR2RGB") {
    return std::make_shared<BGR2RGB>();
  } else {
    std::cerr << "There's unexpected transform(name='" << transform_name
              << "')." << std::endl;
    exit(-1);
  }
}
    
} // namespace name
