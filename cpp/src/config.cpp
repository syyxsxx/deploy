#include <string>

#include "yaml-cpp/yaml.h"

#include "include/deploy/config/ppdet_cfg.h"

namespace Deploy {

bool ConfigPaser::Load(const std::string &cfg_file, const std::string &pp_type) {
    //Load config as a YAML::Node
    det_config = YAML::LoadFile(cfg_file);
    //Paser yaml file
    if (pp_type == 'det') {
        if(!Det_paser(det_config)) {
            std::cerr << "Fail to paser PaddleDection yaml file" << std::endl;
            return false
        }
    }
    
}

bool ConfigPaser::Det_paser(const YAML::Node &det_config) {
    config_["model_format"] = "Paddle";
    //arch support value:YOLO, SSD, RetinaNet, RCNN, Face
    if(det_config["arch"].IsDefined()) {
        config_["model_name"] = det_config["arch"];
    }
    else {
        std::cerr << "Fail to find arch in PaddleDection yaml file" << std::endl;
        return false
    }   
    config_["toolkit"] = "PaddleDetection";
    config_["toolkit_version"] = "Unknown";
    
    if(det_config["label_list"].IsDefined()) {
        int i = 0;
        for (const auto& label : det_config["label_list"]) {
            config_["lables"][i] = label.as<std::string>();
            i++;
        }
    }
    else {
        std::cerr << "Fail to find label_list in  PaddleDection yaml file" << std::endl;
        return false
    }
    //Preprocess support Normalize, Permute, Resize, PadStride
    if(det_config["Preprocess"].IsDefined()) {
        YAML::Node preprocess_info = det_config["Preprocess"];
        for (const auto& preprocess_op : preprocess_info) {
            if(!Det_paser_transforms(preprocess_op)) {
                std::cerr << "Fail to parser PaddleDetection transforms" << std::endl;
                return false
            }
        }    
    }
    else {
        std::cerr << "Fail to find Preprocess in  PaddleDection yaml file" << std::endl;
        return false
    }

}

bool Det_build_transforms(const YAML::Node &preprocess_op) {
    if (preprocess_op["type"] == "Normalize") {
        std::vector<float> mean = preprocess_op.as<std::vector<float>>();
        std::vector<float> scale = preprocess_op.as<std::vector<float>>();
        config_["transforms"]["Normalize"]["is_scale"] = preprocess_op["is_scale"].as<bool>()
        for (int i = 0; i < mean.size(); i++) {
            config_["transforms"]["Normalize"]["mean"].push_back(mean[i]);
            config_["transforms"]["Normalize"]["std"].push_back(std[i]);
            config_["transforms"]["Normalize"]["min_val"].push_back(0);
            config_["transforms"]["Normalize"]["max_val"].push_back(255);
        }        
    }
    else if (preprocess_op["type"] == "Permute") {
        config_["transforms"]["Permute"] = True;
        if (preprocess_op["to_bgr"]) {
            config_["transforms"]["RGB2BRG"] = True;
        }
    }
    else if (preprocess_op["type"] == "Resize") {
        int max_size = preprocess_op["max_size"].as<int>()
        if (max_size !=0 && (config_["model_name"] == "RCNN" || config_["model_name"] == "RetinaNet")) {
            config_["transforms"]["ResizeByShort"]["target_size"] = preprocess_op["target_size"].as<int>();
            config_["transforms"]["ResizeByShort"]["max_size"] = max_size;
            config_["transforms"]["ResizeByShort"]["interp"] = preprocess_op["interp"].as<int>();
        }
        else {
            config_["transforms"]["Resize"]["width"] = preprocess_op["target_size"].as<int>();
            config_["transforms"]["Resize"]["height"] = preprocess_op["target_size"].as<int>();
            config_["transforms"]["Resize"]["max_size"] = max_size;
            config_["transforms"]["Resize"]["interp"] = preprocess_op["interp"].as<int>();
        }
    }
}


}//namespace

