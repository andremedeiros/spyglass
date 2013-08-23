#ifndef ROPENCV_IMAGE_H_
#define ROPENCV_IMAGE_H_

#include "opencv.h"

#include <opencv2/opencv.hpp>

namespace OpenCV {
  namespace Image {
    void define_ruby_class();
    VALUE get_ruby_class();
    VALUE rb_opencv_image_alloc(VALUE self);
    void rb_opencv_image_free(cv::Mat *mat);

    VALUE rb_opencv_image_initialize(VALUE self);
    VALUE rb_opencv_image_crop(VALUE self, VALUE rect);
    VALUE rb_opencv_image_crop_inplace(VALUE self, VALUE rect);
    VALUE rb_opencv_image_dilate(int argc, VALUE *argv, VALUE self);
    VALUE rb_opencv_image_dilate_inplace(int argc, VALUE *argv, VALUE self);
    VALUE rb_opencv_image_erode(int argc, VALUE *argv, VALUE self);
    VALUE rb_opencv_image_erode_inplace(int argc, VALUE *argv, VALUE self);
    VALUE rb_opencv_image_load(int argc, VALUE *argv, VALUE klass);
    VALUE rb_opencv_image_get_cols(VALUE self);
    VALUE rb_opencv_image_get_rows(VALUE self);
    VALUE rb_opencv_image_get_size(VALUE self);
    VALUE rb_opencv_image_write(VALUE self, VALUE filename);
  }
}
#endif // ROPENCV_IMAGE_H_
