#include "rect.h"

static VALUE RectClass;

namespace Spyglass {
  namespace Rect {
    void define_ruby_class() {
      // Class definition
      RectClass = rb_define_class_under(Spyglass::get_ruby_module(), "Rect", rb_cObject);
      rb_define_alloc_func(RectClass, rb_alloc);
      rb_define_method(RectClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 4);

      // Instance methods
      rb_define_method(RectClass, "area", RUBY_METHOD_FUNC(rb_get_area), 0);
      rb_define_method(RectClass, "height", RUBY_METHOD_FUNC(rb_get_height), 0);
      rb_define_method(RectClass, "height=", RUBY_METHOD_FUNC(rb_set_height), 1);
      rb_define_method(RectClass, "point", RUBY_METHOD_FUNC(rb_get_point), 0);
      rb_define_method(RectClass, "size", RUBY_METHOD_FUNC(rb_get_size), 0);
      rb_define_method(RectClass, "width", RUBY_METHOD_FUNC(rb_get_width), 0);
      rb_define_method(RectClass, "width=", RUBY_METHOD_FUNC(rb_set_width), 1);
      rb_define_method(RectClass, "x", RUBY_METHOD_FUNC(rb_get_x), 0);
      rb_define_method(RectClass, "x=", RUBY_METHOD_FUNC(rb_set_x), 1);
      rb_define_method(RectClass, "y", RUBY_METHOD_FUNC(rb_get_y), 0);
      rb_define_method(RectClass, "y=", RUBY_METHOD_FUNC(rb_set_y), 1);
    }

    VALUE get_ruby_class() {
      return RectClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::Rect *rect = new cv::Rect();
      return Data_Wrap_Struct(RectClass, NULL, rb_free, rect);
    }

    static void rb_free(cv::Rect *rect) {
      delete rect;
    }

    static VALUE rb_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height) {
      Check_Type(x, T_FIXNUM);
      Check_Type(y, T_FIXNUM);
      Check_Type(width, T_FIXNUM);
      Check_Type(height, T_FIXNUM);

      cv::Rect *rect = SG_GET_RECT(self);

      rect->x = FIX2INT(x);
      rect->y = FIX2INT(y);
      rect->width = FIX2INT(width);
      rect->height = FIX2INT(height);

      return self;
    }

    static VALUE rb_get_area(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return INT2FIX(rect->area());
    }

    static VALUE rb_get_height(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return INT2FIX(rect->height);
    }

    static VALUE rb_set_height(VALUE self, VALUE height) {
      Check_Type(height, T_FIXNUM);

      cv::Rect *rect = SG_GET_RECT(self);

      rect->height = FIX2INT(height);
      return height;
    }

    static VALUE rb_get_point(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return Point::from_cvrect(rect);
    }

    static VALUE rb_get_size(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return Size::from_cvrect(rect);
    }

    static VALUE rb_get_width(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return INT2FIX(rect->width);
    }

    static VALUE rb_set_width(VALUE self, VALUE width) {
      Check_Type(width, T_FIXNUM);

      cv::Rect *rect = SG_GET_RECT(self);

      rect->width = FIX2INT(width);
      return width;
    }

    static VALUE rb_get_x(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return INT2FIX(rect->x);
    }

    static VALUE rb_set_x(VALUE self, VALUE x) {
      Check_Type(x, T_FIXNUM);

      cv::Rect *rect = SG_GET_RECT(self);

      rect->x = FIX2INT(x);
      return x;
    }

    static VALUE rb_get_y(VALUE self) {
      cv::Rect *rect = SG_GET_RECT(self);
      return INT2FIX(rect->y);
    }

    static VALUE rb_set_y(VALUE self, VALUE y) {
      Check_Type(y, T_FIXNUM);

      cv::Rect *rect = SG_GET_RECT(self);

      rect->y = FIX2INT(y);
      return y;
    }

    VALUE from_cvrect(cv::Rect *rect) {
      cv::Rect *_rect = new cv::Rect(*rect);
      return Data_Wrap_Struct(RectClass, NULL, rb_free, _rect);
    }
  }
}
