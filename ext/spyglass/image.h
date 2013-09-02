#ifndef SPYGLASS_IMAGE_H_
#define SPYGLASS_IMAGE_H_

#include "spyglass.h"

namespace Spyglass {
  namespace Image {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::Mat *mat);
    static VALUE rb_initialize(VALUE self);
    static VALUE rb_crop(VALUE self, VALUE rect);
    static VALUE rb_crop_inplace(VALUE self, VALUE rect);
    static VALUE rb_dilate(int argc, VALUE *argv, VALUE self);
    static VALUE rb_dilate_inplace(int argc, VALUE *argv, VALUE self);
    static VALUE rb_draw_rectangle(VALUE self, VALUE rect);
    static VALUE rb_erode(int argc, VALUE *argv, VALUE self);
    static VALUE rb_erode_inplace(int argc, VALUE *argv, VALUE self);
    static VALUE rb_load(int argc, VALUE *argv, VALUE klass);
    static VALUE rb_get_cols(VALUE self);
    static VALUE rb_get_rows(VALUE self);
    static VALUE rb_get_size(VALUE self);
    static VALUE rb_write(VALUE self, VALUE filename);

    VALUE image_from_cvmat(cv::Mat *mat);
  }
}

#endif // SPYGLASS_IMAGE_H_