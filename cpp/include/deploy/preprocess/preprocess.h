#pragma once

#include <vector>


#include "deploy/blob.h"
#include "transform.h"
#include "config.h" 

class BasePreprocess {
  public:
    BasePreprocess() {}
    
    ~BasePreprocess() {}
  
    virtual bool Init(const YAML::Node &config);

    virtual bool Run(const std::vector<cv::mat> &imgs, std::vector<std::vector<Inblob>> *inputs, std::vector<ShapeInfo> *shape_traces);

    bool BuildTransform(); 
  private:

    std::shared_ptr<Transform> CreateTransform(const std::string& name);
    
    std::vector<std::shared_ptr<Transform>> transforms_;
}
