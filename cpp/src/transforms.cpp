#include "include/deploy/common/transforms.h"

#include <math.h>

#include <iostream>
#include <string>
#include <vector>

namespace Deploy {

std::vector<int> Get_before_shape(const ShapeInfo &shape_trace) {
    std::string name = shape_trace.transform_order[transform_order.size() - 1];
    return shape_trace.shape[name]
}

bool Normalize::Run(cv::Mat* im) {
    
    std::vector<float> range_val;
    for (int c = 0; c < im->channels(); c++) {
        range_val.push_back(max_val_[c] - min_val_[c]);
    }
    std::vector<cv::Mat> split_im;
    cv::split(*im, split_im);
    for (int c = 0; c < im->channels(); c++) {
        cv::subtract(split_im[c], cv::Scalar(min_val_[c]), split_im[c]);
        if (is_scale_){
            float range_val = max_val_[c] - min_val_[c];
            cv::divide(split_im[c], cv::Scalar(range_val), split_im[c]);
        }
        cv::subtract(split_im[c], cv::Scalar(mean_[c]), split_im[c]);
        cv::divide(split_im[c], cv::Scalar(std_[c]), split_im[c]);
    }
    cv::merge(split_im, *im);
    return true;
}

void Normalize::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("Normalize");
    shape_trace->shape["Normalize"] = before_shape;
}


float ResizeByShort::GenerateScale(const int origin_w, const int origin_h) {
  int im_size_max = std::max(origin_w, origin_h);
  int im_size_min = std::min(origin_w, origin_h);
  float scale =
      static_cast<float>(target_size_) / static_cast<float>(im_size_min);
  if (max_size_ > 0) {
    if (round(scale * im_size_max) > max_size_) {
      scale = static_cast<float>(max_size_) / static_cast<float>(im_size_max);
    }
  }
  return scale;
}

bool ResizeByShort::Run(cv::Mat* im) {
    int origin_w = im.cols;
    int origin_h = im.rows;
    float scale = GenerateScale(origin_w, origin_h);
    int width = static_cast<int>(round(scale * im->cols));
    int height = static_cast<int>(round(scale * im->rows));
    cv::resize(*im, *im, cv::Size(width, height), 0, 0, interp_);
    return true;
}

void ResizeByShort::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("ResizeByShort");
    float scale = GenerateScale(before_shape[0], before_shape[1]);
    int width = static_cast<int>(round(scale * im->cols));
    int height = static_cast<int>(round(scale * im->rows));
    std::vector<int> after_shape = {width, height}
    shape_trace->shape["ResizeByShort"] = after_shape;
}

float ResizeByLong::GenerateScale(const cv::Mat& im) {
    int im_size_max = std::max(origin_w, origin_h);
    int im_size_min = std::min(origin_w, origin_h);
    float scale =
        static_cast<float>(target_size_) / static_cast<float>(im_size_max);
    if (max_size_ > 0) {
        if (round(scale * im_size_min) > max_size_) {
            scale = static_cast<float>(max_size_) / static_cast<float>(im_size_min);
        }
    }
    return scale;
}


bool ResizeByLong::Run(cv::Mat* im, ImageBlob* data) {
    int origin_w = im.cols;
    int origin_h = im.rows;
    float scale = GenerateScale(origin_w, origin_h);
    int width = static_cast<int>(round(scale * im->cols));
    int height = static_cast<int>(round(scale * im->rows));
    cv::resize(*im, *im, cv::Size(width, height), 0, 0, interp_);
    return true;
}

void ResizeByLong::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("ResizeByLong");
    float scale = GenerateScale(before_shape[0], before_shape[1]);
    int width = static_cast<int>(round(scale * im->cols));
    int height = static_cast<int>(round(scale * im->rows));
    std::vector<int> after_shape = {width, height}
    shape_trace->shape["ResizeByLong"] = after_shape;
}


bool Resize::Run(cv::Mat* im, ImageBlob* data) {
  if (width_ <= 0 || height_ <= 0) {
    std::cerr << "[Resize] width and height should be greater than 0"
              << std::endl;
    return false;
  }
  cv::resize(
      *im, *im, cv::Size(width_, height_), 0, 0, interp_);
  return true;
}

void Resize::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("Resize");
    std::vector<int> after_shape = {width_, height_}
    shape_trace->shape["Resize"] = after_shape;
}

bool CenterCrop::Run(cv::Mat* im) {
    int height = static_cast<int>(im->rows);
    int width = static_cast<int>(im->cols);
    if (height < height_ || width < width_) {
        std::cerr << "[CenterCrop] Image size less than crop size" << std::endl;
        return false;
    }
    int offset_x = static_cast<int>((width - width_) / 2);
    int offset_y = static_cast<int>((height - height_) / 2);
    cv::Rect crop_roi(offset_x, offset_y, width_, height_);
    *im = (*im)(crop_roi);
    return true;
}

void CenterCrop::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("CenterCrop");
    std::vector<int> after_shape = {width_, height_}
    shape_trace->shape["CenterCrop"] = after_shape;
}

void Padding::GeneralPadding(cv::Mat* im,
                             const std::vector<float> &padding_val,
                             int padding_w, int padding_h) {
  cv::Scalar value;
  if (im->channels() == 1) {
    value = cv::Scalar(padding_val[0]);
  } else if (im->channels() == 2) {
    value = cv::Scalar(padding_val[0], padding_val[1]);
  } else if (im->channels() == 3) {
    value = cv::Scalar(padding_val[0], padding_val[1], padding_val[2]);
  } else if (im->channels() == 4) {
    value = cv::Scalar(padding_val[0], padding_val[1], padding_val[2],
                                  padding_val[3]);
  }
  cv::copyMakeBorder(
  *im,
  *im,
  0,
  padding_h,
  0,
  padding_w,
  cv::BORDER_CONSTANT,
  value);
}

void Padding::MultichannelPadding(cv::Mat* im,
                                  const std::vector<float> &padding_val,
                                  int padding_w, int padding_h) {
  std::vector<cv::Mat> padded_im_per_channel(im->channels());
  for (size_t i = 0; i < im->channels(); i++) {
    const cv::Mat per_channel = cv::Mat(im->rows + padding_h,
                                        im->cols + padding_w,
                                        CV_32FC1,
                                        cv::Scalar(padding_val[i]));
    padded_im_per_channel[i] = per_channel;
  }
  cv::Mat padded_im;
  cv::merge(padded_im_per_channel, padded_im);
  cv::Rect im_roi = cv::Rect(0, 0, im->cols, im->rows);
  im->copyTo(padded_im(im_roi));
  *im = padded_im;
}

bool Padding::Run(cv::Mat* im) {

  int padding_w = 0;
  int padding_h = 0;
  if (width_ > 1 & height_ > 1) {
    padding_w = width_ - im->cols;
    padding_h = height_ - im->rows;
  } else if (coarsest_stride_ >= 1) {
    int h = im->rows;
    int w = im->cols;
    padding_h =
        ceil(h * 1.0 / stride_) * stride_ - im->rows;
    padding_w =
        ceil(w * 1.0 / stride_) * stride_ - im->cols;
  }

  if (padding_h < 0 || padding_w < 0) {
    std::cerr << "[Padding] Computed padding_h=" << padding_h
              << ", padding_w=" << padding_w
              << ", but they should be greater than 0." << std::endl;
    return false;
  }
  if (im->channels() < 5) {
    Padding::GeneralPadding(im, im_value_, padding_w, padding_h);
  } else {
    Padding::MultichannelPadding(im, im_value_, padding_w, padding_h);
  }

  return true;
}

void Padding::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("Padding");
    std::vector<int> after_shape = {width_, height_}
    shape_trace->shape["Padding"] = after_shape;
}

bool Clip::Run(cv::Mat* im) {
  std::vector<cv::Mat> split_im;
  cv::split(*im, split_im);
  for (int c = 0; c < im->channels(); c++) {
    cv::threshold(split_im[c], split_im[c], max_val_[c], max_val_[c],
                  cv::THRESH_TRUNC);
    cv::subtract(cv::Scalar(0), split_im[c], split_im[c]);
    cv::threshold(split_im[c], split_im[c], min_val_[c], min_val_[c],
                  cv::THRESH_TRUNC);
    cv::divide(split_im[c], cv::Scalar(-1), split_im[c]);
  }
  cv::merge(split_im, *im);
  return true;
}

void Clip::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("Clip");
    shape_trace->shape["Clip"] = before_shape;
}

bool BGR2RGB::Run(cv::Mat* im) {
    cv::cvtColor(*im, *im, cv::COLOR_BGR2RGB);
    return true;
}

void BGR2RGB::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("BGR2RGB");
    shape_trace->shape["BGR2RGB"] = before_shape;
}

bool RGB2BGR::Run(cv::Mat* im) {
    cv::cvtColor(*im, *im, cv::COLOR_RGB2BGR);
    return true;
}

void RGB2BGR::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("RGB2BGR");
    shape_trace->shape["RGB2BGR"] = before_shape;
}

bool Permute::Run(cv::Mat* im) {
  int rh = im->rows;
  int rw = im->cols;
  int rc = im->channels();
  float data = im.data;
  for (int i = 0; i < rc; ++i) {
    cv::extractChannel(*im, cv::Mat(rh, rw, CV_32FC1, data + i * rh * rw), i);
  }
  return true;
}

void Permute::Shape_infer(ShapeInfo* shape_trace) {
    std::vector<int> before_shape = Get_before_shape(*shape);
    shape_trace->transform_order.push_back("Permute");
    shape_trace->shape["Permute"] = before_shape;
}


}//namespace