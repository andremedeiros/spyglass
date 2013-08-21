#ifndef ROPENCV_POINT_H_
#define ROPENCV_POINT_H_

#include "opencv.h"

#include <opencv2/opencv.hpp>

namespace OpenCV {
  namespace Point {
    void define_ruby_class();
    VALUE get_ruby_class();
    VALUE rb_opencv_point_alloc(VALUE self);
    void rb_opencv_point_free(cv::Point *point);

    VALUE rb_opencv_point_initialize(VALUE self, VALUE x, VALUE y);
    VALUE rb_opencv_point_get_x(VALUE self);
    VALUE rb_opencv_point_get_y(VALUE self);
    VALUE rb_opencv_point_set_x(VALUE self, VALUE x);
    VALUE rb_opencv_point_set_y(VALUE self, VALUE y);

    VALUE rb_opencv_point_from_cvrect(cv::Rect *rect);
  }
}

#endif
