#pragma once


class BasePostprecess {
  public:
    virtual bool Run(std::vector<Datablob> &outputs, YAML::Node &config, std::vector<ShapeBlob> &shape_traces, std::vector<BaseResult> *results)
