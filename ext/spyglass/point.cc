#include "point.h"

extern VALUE rb_mSpyglass;

VALUE rb_cPoint = Qnil;

void
rb_point_init() {
  // Class definition
  rb_cPoint = rb_define_class_under(rb_mSpyglass, "Point", rb_cObject);
  rb_define_alloc_func(rb_cPoint, rb_point_alloc);
  rb_define_method(rb_cPoint, "initialize", RUBY_METHOD_FUNC(rb_point_initialize), 2);

  // Instance methods
  rb_define_method(rb_cPoint, "x", RUBY_METHOD_FUNC(rb_point_get_x), 0);
  rb_define_method(rb_cPoint, "x=", RUBY_METHOD_FUNC(rb_point_set_x), 1);
  rb_define_method(rb_cPoint, "y", RUBY_METHOD_FUNC(rb_point_get_y), 0);
  rb_define_method(rb_cPoint, "y=", RUBY_METHOD_FUNC(rb_point_set_y), 1);
}

VALUE
rb_point_alloc(VALUE self) {
  cv::Point *point = new cv::Point();
  return Data_Wrap_Struct(rb_cPoint, NULL, rb_point_free, point);
}

void
rb_point_free(cv::Point *point) {
  delete point;
}

VALUE
rb_point_initialize(VALUE self, VALUE r_x, VALUE r_y) {
  Check_Type(r_x, T_FIXNUM);
  Check_Type(r_y, T_FIXNUM);

  cv::Point *point = SG_GET_POINT(self);

  point->x = FIX2INT(r_x);
  point->y = FIX2INT(r_y);

  return self;
}

VALUE
rb_point_get_x(VALUE self) {
  cv::Point *point = SG_GET_POINT(self);
  return INT2FIX(point->x);
}

VALUE
rb_point_set_x(VALUE self, VALUE r_x) {
  Check_Type(r_x, T_FIXNUM);

  cv::Point *point = SG_GET_POINT(self);

  point->x = FIX2INT(r_x);
  return r_x;
}

VALUE
rb_point_get_y(VALUE self) {
  cv::Point *point = SG_GET_POINT(self);
  return INT2FIX(point->y);
}

VALUE
rb_point_set_y(VALUE self, VALUE r_y) {
  Check_Type(r_y, T_FIXNUM);

  cv::Point *point = SG_GET_POINT(self);

  point->y = FIX2INT(r_y);
  return r_y;
}

VALUE
cv_point_to_rb_point(cv::Point *point) {
  return Data_Wrap_Struct(rb_cPoint, NULL, rb_point_free, point);
}

VALUE
cv_rect_to_rb_point(cv::Rect *rect) {
  cv::Point *point = new cv::Point(rect->tl());
  return Data_Wrap_Struct(rb_cPoint, NULL, rb_point_free, point);
}
