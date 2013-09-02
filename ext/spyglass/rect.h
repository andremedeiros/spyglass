#ifndef SPYGLASS_RECT_H_
#define SPYGLASS_RECT_H_

#include "spyglass.h"

namespace Spyglass {
  namespace Rect {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::Rect *rect);
    static VALUE rb_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height);
    static VALUE rb_get_area(VALUE self);
    static VALUE rb_get_height(VALUE self);
    static VALUE rb_get_point(VALUE self);
    static VALUE rb_get_size(VALUE self);
    static VALUE rb_get_width(VALUE self);
    static VALUE rb_get_x(VALUE self);
    static VALUE rb_get_y(VALUE self);
    static VALUE rb_set_height(VALUE self, VALUE height);
    static VALUE rb_set_width(VALUE self, VALUE width);
    static VALUE rb_set_x(VALUE self, VALUE x);
    static VALUE rb_set_y(VALUE self, VALUE y);

    VALUE from_cvrect(cv::Rect *rect);
  }
}

#endif // SPYGLASS_RECT_H_
