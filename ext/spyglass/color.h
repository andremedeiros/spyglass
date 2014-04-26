#ifndef SPYGLASS_COLOR_H_
#define SPYGLASS_COLOR_H_

#include "spyglass.h"

namespace Spyglass {

  SG_GEN_GET_OBJECT_FUNCTION(SG_GET_COLOR, cv::Scalar);

  namespace Color {
    void define_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::Scalar *color);
    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
    static VALUE rb_get_color(VALUE self, VALUE index);
    static VALUE rb_set_color(VALUE self, VALUE index, VALUE color);
    static VALUE rb_is_zeros(VALUE self);
    static VALUE rb_to_a(VALUE self);

    VALUE from_cvscalar(cv::Scalar *color);
  }
}

#endif
