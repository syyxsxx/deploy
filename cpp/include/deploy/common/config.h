#pragma once

#include <string>

#include "yaml-cpp/yaml.h"

namespace Deploy {

class ConfigPaser {
  public:
    ConfigPaser() {}

    ~ConfigPaser() {}

    bool Load(const std::string &cfg_file, const std::string &pp_type);

    
  private:
    bool Det_paser(const YAML::Node &det_config);
    bool Det_paser_transforms(const YAML::Node &preprocess_op);


    YAML::Node config_;
}

}//namespace
