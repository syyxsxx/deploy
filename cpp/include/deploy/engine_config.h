#pragma once


class EngineConfig{

}

class PpiConfig:EngineConfig{
  public:
     bool use_trt;
     bool use_gpu;
     bool use_mkl;
     int  thread_num;
}
