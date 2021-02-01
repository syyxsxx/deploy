#pragma once

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "blob.h"

class Transform {
  public:

    virtual void Init(const YAML::Node& item) = 0;
  
    virtual void Shape_infer(ShapeInfo* shape) = 0;
  
    virtual bool Run(cv::Mat* im) = 0;
};

class Normalize : public Transform {
  public:
    virtual void Init(const YAML::Node& item) {
      mean_ = item["mean"].as<std::vector<float>>();
      std_ = item["std"].as<std::vector<float>>();
      if (item["is_scale"].IsDefined()) {
        is_scale_ = item["is_scale"];
      }
      if (item["min_val"].IsDefined()) {
        min_val_ = item["min_val"].as<std::vector<float>>();
      } else {
        min_val_ = std::vector<float>(mean_.size(), 0.);
      }
      if (item["max_val"].IsDefined()) {
        max_val_ = item["max_val"].as<std::vector<float>>();
      } else {
        max_val_ = std::vector<float>(mean_.size(), 255.);
      }
    }
    virtual bool Run(cv::Mat* im);
    virtual void Shape_infer(ShapeInfo* shape);
  
  private:
    bool is_scale_;
    std::vector<float> mean_;
    std::vector<float> std_;
    std::vector<float> min_val_;
    std::vector<float> max_val_;
}

/*interp_: std::vector<int> interpolations = {
  cv::INTER_LINEAR, 
  cv::INTER_NEAREST, 
  cv::INTER_AREA, 
  cv::INTER_CUBIC, 
  cv::INTER_LANCZOS4}*/
class ResizeByShort : public Transform {
  public:
    virtual void Init(const YAML::Node& item) {
      target_size_ = item["target_size"].as<int>();
      if (item["interp"].IsDefined()) {
        interp_ = item["interp"].as<int>();
      }
      else {
        interp_ = 0;
      }
      if (item["max_size"].IsDefined()) {
        max_size_ = item["max_size"].as<int>();
      } else {
        max_size_ = -1;
      }
    }
  virtual bool Run(cv::Mat* im, ImageBlob* data);

 private:
    float GenerateScale(const cv::Mat& im);
    int target_size_;
    int max_size_;
    int interp_;
};


class ResizeByLong : public Transform {
  public:
  virtual void Init(const YAML::Node& item) {
    target_size_ = item["target_size"].as<int>();
    if (item["interp"].IsDefined()) {
        interp_ = item["interp"].as<int>();
    } else {
      interp_ = 0;
    }
    if (item["max_size"].IsDefined()) {
        max_size_ = item["max_size"].as<int>();
    } else {
      max_size_ = -1;
    }
  }
  virtual bool Run(cv::Mat* im);

  private:
  float GenerateScale(const cv::Mat& im);
  int target_size_;
  int max_size_;
  int interp_;
};

class Resize : public Transform {
 public:
  virtual void Init(const YAML::Node& item) {
    if (item["interp"].IsDefined()) {
      interp_ = item["interp"].as<int>();
    }
    height_ = item["height"].as<int>();
    width_ = item["width"].as<int>();
    if (height_ <= 0 || width_ <= 0) {
      std::cerr << "[Resize] target_size should greater than 0" << std::endl;
      exit(-1);
    }
  }
  virtual bool Run(cv::Mat* im);

 private:
  int height_;
  int width_;
  int interp_;
};

class BGR2RGB : public Transform {
  public:
    virtual void Init(const YAML::Node& item) {
    }
    virtual bool Run(cv::Mat* im);
}

class RGB2BGR : public Transform {
  public:
    virtual void Init(const YAML::Node& item) {
    }
    virtual bool Run(cv::Mat* im);
}

class Padding : public Transform {
 public:
  virtual void Init(const YAML::Node& item) {
    if (item["stride"].IsDefined()) {
      stride_ = item["stride"].as<int>();
      if (coarsest_stride_ < 1) {
        std::cerr << "[Padding] coarest_stride should greater than 0"
                  << std::endl;
        exit(-1);
      }
    }
    if (item["width"].IsDefined() && item["height"].IsDefined()) {
      width_ = item["width"].as<int>();
      height_ = item["height"].as<int>();
    }
    if (item["im_padding_value"].IsDefined()) {
      im_value_ = item["im_padding_value"].as<std::vector<float>>();
    } else {
      im_value_ = {0, 0, 0};
    }
  }
  virtual bool Run(cv::Mat* im);
  virtual void GeneralPadding(cv::Mat* im,
                              const std::vector<float> &padding_val,
                              int padding_w, int padding_h);
  virtual void MultichannelPadding(cv::Mat* im,
                                   const std::vector<float> &padding_val,
                                   int padding_w, int padding_h);
 private:
  int stride_ = -1;
  int width_ = 0;
  int height_ = 0;
  std::vector<float> im_value_;
};

class CenterCrop : public Transform {
 public:
  virtual void Init(const YAML::Node& item) {
    height_ = item["width"].as<int>();
    width_ = item["height"].as<int>();
  }
  virtual bool Run(cv::Mat* im);

 private:
  int height_;
  int width_;
};


class Clip : public Transform {
 public:
  virtual void Init(const YAML::Node& item) {
    min_val_ = item["min_val"].as<std::vector<float>>();
    max_val_ = item["max_val"].as<std::vector<float>>();
  }

  virtual bool Run(cv::Mat* im);

 private:
  std::vector<float> min_val_;
  std::vector<float> max_val_;
};