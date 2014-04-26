#include "rect.h"

extern VALUE SpyglassModule;
extern VALUE PointClass;

VALUE RectClass = Qnil;

void rb_rect_init() {
  // Class definition
  RectClass = rb_define_class_under(SpyglassModule, "Rect", rb_cObject);
  rb_define_alloc_func(RectClass, rb_rect_alloc);
  rb_define_method(RectClass, "initialize", RUBY_METHOD_FUNC(rb_rect_initialize), 4);

  // Instance methods
  rb_define_method(RectClass, "area", RUBY_METHOD_FUNC(rb_rect_get_area), 0);
  rb_define_method(RectClass, "center", RUBY_METHOD_FUNC(rb_rect_get_center), 0);
  rb_define_method(RectClass, "contains?", RUBY_METHOD_FUNC(rb_rect_contains), 1);
  rb_define_method(RectClass, "height", RUBY_METHOD_FUNC(rb_rect_get_height), 0);
  rb_define_method(RectClass, "height=", RUBY_METHOD_FUNC(rb_rect_set_height), 1);
  rb_define_method(RectClass, "point", RUBY_METHOD_FUNC(rb_rect_get_point), 0);
  rb_define_method(RectClass, "size", RUBY_METHOD_FUNC(rb_rect_get_size), 0);
  rb_define_method(RectClass, "to_a", RUBY_METHOD_FUNC(rb_rect_to_a), 0);
  rb_define_method(RectClass, "width", RUBY_METHOD_FUNC(rb_rect_get_width), 0);
  rb_define_method(RectClass, "width=", RUBY_METHOD_FUNC(rb_rect_set_width), 1);
  rb_define_method(RectClass, "x", RUBY_METHOD_FUNC(rb_rect_get_x), 0);
  rb_define_method(RectClass, "x=", RUBY_METHOD_FUNC(rb_rect_set_x), 1);
  rb_define_method(RectClass, "y", RUBY_METHOD_FUNC(rb_rect_get_y), 0);
  rb_define_method(RectClass, "y=", RUBY_METHOD_FUNC(rb_rect_set_y), 1);
}

VALUE
rb_rect_alloc(VALUE self) {
  cv::Rect *rect = new cv::Rect();
  return Data_Wrap_Struct(RectClass, NULL, rb_rect_free, rect);
}

void
rb_rect_free(cv::Rect *rect) {
  delete rect;
}

VALUE
rb_rect_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height) {
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

VALUE
rb_rect_contains(VALUE self, VALUE other) {
  cv::Rect *rect = SG_GET_RECT(self);
  if(CLASS_OF(other) == PointClass) {
    cv::Point *point = SG_GET_POINT(other);
    bool within_x = (rect->x > point->x || rect->x + rect->width < point->x);
    bool within_y = (rect->y > point->y || rect->y + rect->height < point->y);

    return (!within_x && !within_y) ? Qtrue : Qfalse;
  } else {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Point)",
        rb_obj_classname(other));
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
rb_rect_set_height(VALUE self, VALUE height) {
  Check_Type(height, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->height = FIX2INT(height);
  return height;
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
rb_rect_set_width(VALUE self, VALUE width) {
  Check_Type(width, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->width = FIX2INT(width);
  return width;
}

VALUE
rb_rect_get_x(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->x);
}

VALUE
rb_rect_set_x(VALUE self, VALUE x) {
  Check_Type(x, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->x = FIX2INT(x);
  return x;
}

VALUE
rb_rect_get_y(VALUE self) {
  cv::Rect *rect = SG_GET_RECT(self);
  return INT2FIX(rect->y);
}

VALUE
rb_rect_set_y(VALUE self, VALUE y) {
  Check_Type(y, T_FIXNUM);

  cv::Rect *rect = SG_GET_RECT(self);

  rect->y = FIX2INT(y);
  return y;
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
  return Data_Wrap_Struct(RectClass, NULL, rb_rect_free, rect);
}
