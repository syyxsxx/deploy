#pragma once

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
