#pragma once

#include ""

class PPInference_Model {
  public:
    bool Init(const string &model_dir, const string &cfg_file, Engine_config &engine_config);
  
    bool Predict(std::vector<cv::Mat> &imgs, std::vector<DetResult> *results);
    
    bool Predict(std::vector<cv::Mat> &imgs, std::vector<ClsResult> *results);

    bool Predict(std::vector<cv::Mat> &imgs, std::vector<SegResult> *results);

    bool Predict(std::vector<cv::Mat> &imgs, std::vector<XResult> *results);

    bool Predict(std::vector<cv::Mat> &imgs, std::vector<OCRResult> *results);

  private:
    YAML::Node config_;
    
}
