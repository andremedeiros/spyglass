#include "rect.h"

static VALUE RectClass;

namespace OpenCV {
  namespace Rect {
    void define_ruby_class() {
      // Class definition
      RectClass = rb_define_class_under(OpenCV::get_ruby_module(), "Rect", rb_cObject);
      rb_define_alloc_func(RectClass, rb_opencv_rect_alloc);
      rb_define_method(RectClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_rect_initialize), 4);

      // Instance methods
      rb_define_method(RectClass, "area", RUBY_METHOD_FUNC(rb_opencv_rect_get_area), 0);
      rb_define_method(RectClass, "height", RUBY_METHOD_FUNC(rb_opencv_rect_get_height), 0);
      rb_define_method(RectClass, "height=", RUBY_METHOD_FUNC(rb_opencv_rect_set_height), 1);
      rb_define_method(RectClass, "point", RUBY_METHOD_FUNC(rb_opencv_rect_get_point), 0);
      rb_define_method(RectClass, "size", RUBY_METHOD_FUNC(rb_opencv_rect_get_size), 0);
      rb_define_method(RectClass, "width", RUBY_METHOD_FUNC(rb_opencv_rect_get_width), 0);
      rb_define_method(RectClass, "width=", RUBY_METHOD_FUNC(rb_opencv_rect_set_width), 1);
      rb_define_method(RectClass, "x", RUBY_METHOD_FUNC(rb_opencv_rect_get_x), 0);
      rb_define_method(RectClass, "x=", RUBY_METHOD_FUNC(rb_opencv_rect_set_x), 1);
      rb_define_method(RectClass, "y", RUBY_METHOD_FUNC(rb_opencv_rect_get_y), 0);
      rb_define_method(RectClass, "y=", RUBY_METHOD_FUNC(rb_opencv_rect_set_y), 1);
    }

    VALUE get_ruby_class() {
      return RectClass;
    }

    VALUE rb_opencv_rect_alloc(VALUE self) {
      cv::Rect *rect = new cv::Rect();
      return Data_Wrap_Struct(RectClass, NULL, rb_opencv_rect_free, rect);
    }

    void rb_opencv_rect_free(cv::Rect *rect) {
      delete rect;
    }

    VALUE rb_opencv_rect_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height) {
      Check_Type(x, T_FIXNUM);
      Check_Type(y, T_FIXNUM);
      Check_Type(width, T_FIXNUM);
      Check_Type(height, T_FIXNUM);

      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      rect->x = FIX2INT(x);
      rect->y = FIX2INT(y);
      rect->width = FIX2INT(width);
      rect->height = FIX2INT(height);

      return self;
    }

    VALUE rb_opencv_rect_get_area(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      return INT2FIX(rect->area());
    }

    VALUE rb_opencv_rect_get_height(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      return INT2FIX(rect->height);
    }

    VALUE rb_opencv_rect_set_height(VALUE self, VALUE height) {
      Check_Type(height, T_FIXNUM);

      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      rect->height = FIX2INT(height);
      return height;
    }

    VALUE rb_opencv_rect_get_point(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);
      return OpenCV::Point::rb_opencv_point_from_cvrect(rect);
    }

    VALUE rb_opencv_rect_get_size(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);
      return OpenCV::Size::rb_opencv_size_from_cvrect(rect);
    }

    VALUE rb_opencv_rect_get_width(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      return INT2FIX(rect->width);
    }

    VALUE rb_opencv_rect_set_width(VALUE self, VALUE width) {
      Check_Type(width, T_FIXNUM);

      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      rect->width = FIX2INT(width);
      return width;
    }

    VALUE rb_opencv_rect_get_x(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      return INT2FIX(rect->x);
    }

    VALUE rb_opencv_rect_set_x(VALUE self, VALUE x) {
      Check_Type(x, T_FIXNUM);

      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      rect->x = FIX2INT(x);
      return x;
    }

    VALUE rb_opencv_rect_get_y(VALUE self) {
      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      return INT2FIX(rect->y);
    }

    VALUE rb_opencv_rect_set_y(VALUE self, VALUE y) {
      Check_Type(y, T_FIXNUM);

      cv::Rect *rect;
      Data_Get_Struct(self, cv::Rect, rect);

      rect->y = FIX2INT(y);
      return y;
    }

    VALUE rb_opencv_rect_from_cvrect(cv::Rect *rect) {
      cv::Rect *_rect = new cv::Rect(*rect);
      return Data_Wrap_Struct(RectClass, NULL, rb_opencv_rect_free, _rect);
    }
  }
}
