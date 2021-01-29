#pragma once

#include "include/deploy/preprocess/preprocess.h"
#include "include/deploy/common/blob.h"


namespace Deploy {

class DetProecess : pubulic BasePreprocess {
    public:
    DetProecess() {}
    
    ~DetProecess() {}
  
    virtual bool Init(const ConfigParser &parser);

    virtual bool DetProecess::Run(const std::vector<cv::Mat> &imgs, std::vector<std::vector<Inblob>> *inputs, std::vector<ShapeInfo> *shape_traces);
    
}

}//namespace