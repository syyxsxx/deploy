#pragma once

#include "blob.h"

#include "paddle_inference_api.h"

class PpInferenceEngine{
  public:
    void Init(std::string model_dir, PPI_config &config)

    void Infer(std::vector<std::vector<std::DateBlob,DateBlob>> &inputs, std::vector<std::vector<DateBlob>> *outputs)
  private:
    std::unique_ptr<paddle::PaddlePredictor> predictor;
}
