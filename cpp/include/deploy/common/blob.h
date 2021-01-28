#pragma once

include <vector>
include <map>

namespace Deploy {

class DataBlob{
  public:
    // data
    char *data;
    
    // data name
    std::string name;

    // data shape
    std::vector<int> shape;

    // data dtype
    int dtype;

}

class ShapeInfo{
  public:
    
    // shape trace
    std::map<std::string transform, std::vector<int>> shape;
    
    // transform order
    std::vector<std::string> transform_order;
   
}

}//namespace
