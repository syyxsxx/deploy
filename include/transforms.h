#pragma once

#include <blob.h>

class Transform {
  public:
  
  virtual void Init(const YAML::Node& item) = 0;
  
  virtual void Shape_infer(ShapeBlob* shape) = 0;
  
  virtual bool Run(cv::Mat* im) = 0
};
