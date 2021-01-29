#pragma once

#include "ppdet_pre_proc.h"


namespace Deploy {

bool DetProecess::Init(const ConfigParser &parser) {
    BuildTransform(parser);
}

virtual bool DetProecess::Run(const std::vector<cv::Mat> &imgs, std::vector<std::vector<Inblob>> *inputs, std::vector<ShapeInfo> *shape_traces){
    inputs.clear();
    for (int i=0; i < imgs.size(); i++) {
        ShapeInfo im_shape;
        cv::Mat im = imgs[i].clone();
        //(w,h)
        std::vector<int> size = {im.cols, im.rows};
        im_shape.transform_order.push_back("Origin")
        im_shape.shape["Origin"] = size;
        for (int j=0; j < transforms.size(); j++) {
            if (!transforms[j]->Run(&im)) {
                std::cerr << "Apply transforms to image failed!" << std::endl;
                return false;
            }
            if (!transforms[j]->Shape_infer(&im_shape)) {
                std::cerr << "Apply shape inference failed!" << std::endl;
                return false;
            }
        }

    }
}

}