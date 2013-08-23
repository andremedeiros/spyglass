#ifndef ROPENCV_RECT_H_
#define ROPENCV_RECT_H_

#include "opencv.h"

#include <opencv2/opencv.hpp>

namespace OpenCV {
  namespace Rect {
    void define_ruby_class();
    VALUE get_ruby_class();
    VALUE rb_opencv_rect_alloc(VALUE self);
    void rb_opencv_rect_free(cv::Rect *rect);

    VALUE rb_opencv_rect_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height);
    VALUE rb_opencv_rect_get_area(VALUE self);
    VALUE rb_opencv_rect_get_height(VALUE self);
    VALUE rb_opencv_rect_get_point(VALUE self);
    VALUE rb_opencv_rect_get_size(VALUE self);
    VALUE rb_opencv_rect_get_width(VALUE self);
    VALUE rb_opencv_rect_get_x(VALUE self);
    VALUE rb_opencv_rect_get_y(VALUE self);
    VALUE rb_opencv_rect_set_height(VALUE self, VALUE height);
    VALUE rb_opencv_rect_set_width(VALUE self, VALUE width);
    VALUE rb_opencv_rect_set_x(VALUE self, VALUE x);
    VALUE rb_opencv_rect_set_y(VALUE self, VALUE y);

    VALUE rb_opencv_rect_from_cvrect(cv::Rect *rect);
  }
}

#endif
