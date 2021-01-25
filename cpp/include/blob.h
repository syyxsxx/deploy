#pragma once

include <vector>
include <map>

class InBlob{
  public:
    // data
    template <typename T>
    std::vector<T> data;

    // data name
    std::string name;

    // data shape
    std::vector<int> shape;

    // data dtype
    int dtype;

}


class OutBlob{
  public:
    // output data
    char *output_data;
  
    // output name
    std::string name; 
    
    // output shape
    std::vector<int> shape;
}


class ShapeBlob{
  public:
    
    // shape trace
    std::map<std::string transform, std::vector<int>> shape;
    
    // transform order
    std::vector<std::string> transform_order;
   
}
