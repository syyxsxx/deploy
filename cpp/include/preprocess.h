#pragma once

include <vector>


include "blob.h"
include "transform.h"
include "config.h" 

class BasePreprocess {
  public:
    BasePreprocess() {}
    
    ~BasePreprocess() {}
  
    bool Init(YAML::Node &config);

   virtual bool Run(std::vector<cv::mat> &imgs, YANL::Node config, std::vector<std::vector<Inblob>> *inputs, std::vector<Shapeblob> shape_traces)
  
  private:
    std::shared_ptr<Transform> CreateTransform(const std::string& name);
    
    std::vector<std::shared_ptr<Transform>> transforms_;
}
