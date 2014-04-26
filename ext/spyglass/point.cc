#include "point.h"

extern VALUE SpyglassModule;

VALUE PointClass = Qnil;

namespace Spyglass {
  namespace Point {
    void define_ruby_class() {
      // Class definition
      PointClass = rb_define_class_under(SpyglassModule, "Point", rb_cObject);
      rb_define_alloc_func(PointClass, rb_alloc);
      rb_define_method(PointClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 2);

      // Instance methods
      rb_define_method(PointClass, "x", RUBY_METHOD_FUNC(rb_get_x), 0);
      rb_define_method(PointClass, "x=", RUBY_METHOD_FUNC(rb_set_x), 1);
      rb_define_method(PointClass, "y", RUBY_METHOD_FUNC(rb_get_y), 0);
      rb_define_method(PointClass, "y=", RUBY_METHOD_FUNC(rb_set_y), 1);
    }

    static VALUE rb_alloc(VALUE self) {
      cv::Point *point = new cv::Point();
      return Data_Wrap_Struct(PointClass, NULL, rb_free, point);
    }

    static void rb_free(cv::Point *point) {
      delete point;
    }

    static VALUE rb_initialize(VALUE self, VALUE x, VALUE y) {
      Check_Type(x, T_FIXNUM);
      Check_Type(y, T_FIXNUM);

      cv::Point *point = SG_GET_POINT(self);

      point->x = FIX2INT(x);
      point->y = FIX2INT(y);

      return self;
    }

    static VALUE rb_get_x(VALUE self) {
      cv::Point *point = SG_GET_POINT(self);
      return INT2FIX(point->x);
    }

    static VALUE rb_set_x(VALUE self, VALUE x) {
      Check_Type(x, T_FIXNUM);

      cv::Point *point = SG_GET_POINT(self);

      point->x = FIX2INT(x);
      return x;
    }

    static VALUE rb_get_y(VALUE self) {
      cv::Point *point = SG_GET_POINT(self);
      return INT2FIX(point->y);
    }

    static VALUE rb_set_y(VALUE self, VALUE y) {
      Check_Type(y, T_FIXNUM);

      cv::Point *point = SG_GET_POINT(self);

      point->y = FIX2INT(y);
      return y;
    }

    VALUE from_cvpoint(cv::Point *point) {
      return Data_Wrap_Struct(PointClass, NULL, rb_free, point);
    }

    VALUE from_cvrect(cv::Rect *rect) {
      cv::Point *point = new cv::Point(rect->tl());
      return Data_Wrap_Struct(PointClass, NULL, rb_free, point);
    }
  }
}
