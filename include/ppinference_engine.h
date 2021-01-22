#pragma once

#include "blob.h"

#include "paddle_inference_api.h"

class PPInference_Engine{
  public:
    void Init(std::string model_dir, PPI_config &config)

    void Infer(std::vector<std::map<std::string,InBlob>> &inputs, std::vector<OutBlob> *outputs)
  private:
    std::unique_ptr<paddle::PaddlePredictor> predictor_
}
