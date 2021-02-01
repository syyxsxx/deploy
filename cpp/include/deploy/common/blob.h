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

    /* data dtype
    0: int
    1: float
    2: double
    3: char
    4: unsigned int
    5: unsigned long int
    */
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
