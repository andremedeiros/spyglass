#include "rect.h"

extern VALUE rb_mSpyglass;
extern VALUE rb_cPoint;

VALUE rb_cRect = Qnil;

void rb_rect_init() {
  // Class definition
  rb_cRect = rb_define_class_under(rb_mSpyglass, "Rect", rb_cObject);
  rb_define_alloc_func(rb_cRect, rb_rect_alloc);
  rb_define_method(rb_cRect, "initialize", RUBY_METHOD_FUNC(rb_rect_initialize), 4);

  // Instance methods
  rb_define_method(rb_cRect, "area", RUBY_METHOD_FUNC(rb_rect_get_area), 0);
  rb_define_method(rb_cRect, "center", RUBY_METHOD_FUNC(rb_rect_get_center), 0);
  rb_define_method(rb_cRect, "contains?", RUBY_METHOD_FUNC(rb_rect_contains), 1);
  rb_define_method(rb_cRect, "height", RUBY_METHOD_FUNC(rb_rect_get_height), 0);
  rb_define_method(rb_cRect, "height=", RUBY_METHOD_FUNC(rb_rect_set_height), 1);
  rb_define_method(rb_cRect, "point", RUBY_METHOD_FUNC(rb_rect_get_point), 0);
  rb_define_method(rb_cRect, "size", RUBY_METHOD_FUNC(rb_rect_get_size), 0);
  rb_define_method(rb_cRect, "to_a", RUBY_METHOD_FUNC(rb_rect_to_a), 0);
  rb_define_method(rb_cRect, "width", RUBY_METHOD_FUNC(rb_rect_get_width), 0);
  rb_define_method(rb_cRect, "width=", RUBY_METHOD_FUNC(rb_rect_set_width), 1);
  rb_define_method(rb_cRect, "x", RUBY_METHOD_FUNC(rb_rect_get_x), 0);
  rb_define_method(rb_cRect, "x=", RUBY_METHOD_FUNC(rb_rect_set_x), 1);
  rb_define_method(rb_cRect, "y", RUBY_METHOD_FUNC(rb_rect_get_y), 0);
  rb_define_method(rb_cRect, "y=", RUBY_METHOD_FUNC(rb_rect_set_y), 1);
}

VALUE
rb_rect_alloc(VALUE self) {
  cv::Rect *rect = new cv::Rect();
  return Data_Wrap_Struct(rb_cRect, NULL, rb_rect_free, rect);
}

void
rb_rect_free(cv::Rect *rect) {
  delete rect;
}

VALUE
rb_rect_initialize(VALUE self, VALUE r_x, VALUE r_y, VALUE r_width, VALUE r_height) {
  Check_Type(r_x, T_FIXNUM);
  Check_Type(r_y, T_FIXNUM);
  Check_Type(r_width, T_FIXNUM);
  Check_Type(r_height, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->x = FIX2INT(r_x);
  rect->y = FIX2INT(r_y);
  rect->width = FIX2INT(r_width);
  rect->height = FIX2INT(r_height);

  return self;
}

VALUE
rb_rect_contains(VALUE self, VALUE r_other) {
  cv::Rect *rect = SG_GET_RECT(self);
  if(CLASS_OF(r_other) == rb_cPoint) {
    cv::Point *point = SG_GET_POINT(r_other);
    bool within_x = (rect->x > point->x || rect->x + rect->width < point->x);
    bool within_y = (rect->y > point->y || rect->y + rect->height < point->y);

    return (!within_x && !within_y) ? Qtrue : Qfalse;
  } else {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Point)",
        rb_obj_classname(r_other));
  }
}

VALUE
rb_rect_get_area(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->area());
}

VALUE
rb_rect_get_center(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  cv::Point *point = new cv::Point();

  point->x = rect->x + (rect->width / 2);
  point->y = rect->y + (rect->height / 2);

  return cv_point_to_rb_point(point);
}

VALUE
rb_rect_get_height(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->height);
}

VALUE
rb_rect_set_height(VALUE self, VALUE r_height) {
  Check_Type(r_height, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->height = FIX2INT(r_height);
  return r_height;
}

VALUE
rb_rect_get_point(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return cv_rect_to_rb_point(rect);
}

VALUE
rb_rect_get_size(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return cv_rect_to_rb_size(rect);
}

VALUE
rb_rect_get_width(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->width);
}

VALUE
rb_rect_set_width(VALUE self, VALUE r_width) {
  Check_Type(r_width, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->width = FIX2INT(r_width);
  return r_width;
}

VALUE
rb_rect_get_x(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->x);
}

VALUE
rb_rect_set_x(VALUE self, VALUE r_x) {
  Check_Type(r_x, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->x = FIX2INT(r_x);
  return r_x;
}

VALUE
rb_rect_get_y(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->y);
}

VALUE
rb_rect_set_y(VALUE self, VALUE r_y) {
  Check_Type(r_y, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->y = FIX2INT(r_y);
  return r_y;
}

VALUE
rb_rect_to_a(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);

  VALUE ary = rb_ary_new2(4);
  rb_ary_store(ary, 0, INT2NUM(rect->x));
  rb_ary_store(ary, 1, INT2NUM(rect->y));
  rb_ary_store(ary, 2, INT2NUM(rect->width));
  rb_ary_store(ary, 3, INT2NUM(rect->height));

  return ary;
}

VALUE
cv_rect_to_rb_rect(cv::Rect *rect) {
  return Data_Wrap_Struct(rb_cRect, NULL, rb_rect_free, rect);
}
