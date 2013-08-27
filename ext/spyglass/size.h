#ifndef SPYGLASS_SIZE_H_
#define SPYGLASS_SIZE_H_

#include "spyglass.h"

namespace Spyglass {
  namespace Size {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::Size *size);
    static VALUE rb_initialize(VALUE self, VALUE width, VALUE height);
    static VALUE rb_get_area(VALUE self);
    static VALUE rb_get_width(VALUE self);
    static VALUE rb_get_height(VALUE self);
    static VALUE rb_set_width(VALUE self, VALUE width);
    static VALUE rb_set_height(VALUE self, VALUE height);

    VALUE size_from_cvmat(cv::Mat *mat);
    VALUE size_from_cvrect(cv::Rect *rect);
  }
}

#endif // SPYGLASS_SIZE_H_
