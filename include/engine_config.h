#pragma once


class Engine_config{

}

class PPI_onfig:Engine_cofig{
  public:
     bool use_trt;
     bool use_gpu;
     bool use_mkl;
     int  thread_num;
}
