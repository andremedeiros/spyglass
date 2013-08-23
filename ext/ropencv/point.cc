#include "point.h"

static VALUE PointClass;

namespace OpenCV {
  namespace Point {
    void define_ruby_class() {
      // Class definition
      PointClass = rb_define_class_under(OpenCV::get_ruby_module(), "Point", rb_cObject);
      rb_define_alloc_func(PointClass, rb_opencv_point_alloc);
      rb_define_method(PointClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_point_initialize), 2);

      // Instance methods
      rb_define_method(PointClass, "x", RUBY_METHOD_FUNC(rb_opencv_point_get_x), 0);
      rb_define_method(PointClass, "x=", RUBY_METHOD_FUNC(rb_opencv_point_set_x), 1);
      rb_define_method(PointClass, "y", RUBY_METHOD_FUNC(rb_opencv_point_get_y), 0);
      rb_define_method(PointClass, "y=", RUBY_METHOD_FUNC(rb_opencv_point_set_y), 1);
    }

    VALUE get_ruby_class() {
      return PointClass;
    }

    VALUE rb_opencv_point_alloc(VALUE self) {
      cv::Point *point = new cv::Point();
      return Data_Wrap_Struct(PointClass, NULL, rb_opencv_point_free, point);
    }

    void rb_opencv_point_free(cv::Point *point) {
      delete point;
    }

    VALUE rb_opencv_point_initialize(VALUE self, VALUE x, VALUE y) {
      Check_Type(x, T_FIXNUM);
      Check_Type(y, T_FIXNUM);

      cv::Point *point;
      Data_Get_Struct(self, cv::Point, point);

      point->x = FIX2INT(x);
      point->y = FIX2INT(y);

      return self;
    }

    VALUE rb_opencv_point_get_x(VALUE self) {
      cv::Point *point;
      Data_Get_Struct(self, cv::Point, point);

      return INT2FIX(point->x);
    }

    VALUE rb_opencv_point_set_x(VALUE self, VALUE x) {
      Check_Type(x, T_FIXNUM);

      cv::Point *point;
      Data_Get_Struct(self, cv::Point, point);

      point->x = FIX2INT(x);
      return x;
    }

    VALUE rb_opencv_point_get_y(VALUE self) {
      cv::Point *point;
      Data_Get_Struct(self, cv::Point, point);

      return INT2FIX(point->y);
    }

    VALUE rb_opencv_point_set_y(VALUE self, VALUE y) {
      Check_Type(y, T_FIXNUM);

      cv::Point *point;
      Data_Get_Struct(self, cv::Point, point);

      point->y = FIX2INT(y);
      return y;
    }

    VALUE rb_opencv_point_from_cvrect(cv::Rect *rect) {
      cv::Point *point = new cv::Point(rect->tl());
      return Data_Wrap_Struct(PointClass, NULL, rb_opencv_point_free, point);
    }
  }
}
