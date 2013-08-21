#ifndef ROPENCV_SIZE_H_
#define ROPENCV_SIZE_H_

#include "opencv.h"

#include <opencv2/opencv.hpp>

namespace OpenCV {
  namespace Size {
    void define_ruby_class();
    VALUE get_ruby_class();
    VALUE rb_opencv_size_alloc(VALUE self);
    void rb_opencv_size_free(cv::Size *size);

    VALUE rb_opencv_size_initialize(VALUE self, VALUE width, VALUE height);
    VALUE rb_opencv_size_get_area(VALUE self);
    VALUE rb_opencv_size_get_width(VALUE self);
    VALUE rb_opencv_size_get_height(VALUE self);
    VALUE rb_opencv_size_set_width(VALUE self, VALUE width);
    VALUE rb_opencv_size_set_height(VALUE self, VALUE height);

    VALUE rb_opencv_size_from_cvmat(cv::Mat *mat);
    VALUE rb_opencv_size_from_cvrect(cv::Rect *rect);
  }
}

#endif // ROPENCV_SIZE_H_
