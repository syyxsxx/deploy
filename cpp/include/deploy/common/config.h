#pragma once

#include <string>

#include "yaml-cpp/yaml.h"

namespace Deploy {

class ConfigParser {
  public:
    ConfigParser() {}

    ~ConfigParser() {}

    bool Load(const std::string &cfg_file, const std::string &pp_type);

    template <typename T>
    const T& Get(const string &name) const {
      return config_.as<T>();
    }

    
  private:
    bool Det_parser(const YAML::Node &det_config);

    bool Det_parser_transforms(const YAML::Node &preprocess_op);

    YAML::Node config_;
}

}//namespace
