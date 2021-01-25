#pragma once

#include "yaml-cpp/yaml.h"


class ConfigPaser {
  public:
    bool load_config(const std::string &cfg_file, YAML::Node *config)
  private:
    bool config_paser(YAML::Node &config)
}
