#include "size.h"

extern VALUE rb_mSpyglass;

VALUE rb_cSize = Qnil;

void
rb_size_init() {
  // Class definition
  rb_cSize = rb_define_class_under(rb_mSpyglass, "Size", rb_cObject);
  rb_define_alloc_func(rb_cSize, rb_size_alloc);
  rb_define_method(rb_cSize, "initialize", RUBY_METHOD_FUNC(rb_size_initialize), 2);

  // Instance methods
  rb_define_method(rb_cSize, "area", RUBY_METHOD_FUNC(rb_size_get_area), 0);
  rb_define_method(rb_cSize, "height", RUBY_METHOD_FUNC(rb_size_get_height), 0);
  rb_define_method(rb_cSize, "height=", RUBY_METHOD_FUNC(rb_size_set_height), 1);
  rb_define_method(rb_cSize, "width", RUBY_METHOD_FUNC(rb_size_get_width), 0);
  rb_define_method(rb_cSize, "width=", RUBY_METHOD_FUNC(rb_size_set_width), 1);
}

VALUE
rb_size_alloc(VALUE self) {
  cv::Size *size = new cv::Size();
  return Data_Wrap_Struct(rb_cSize, NULL, rb_size_free, size);
}

void
rb_size_free(cv::Size *size) {
  delete size;
}

VALUE
rb_size_initialize(VALUE self, VALUE r_width, VALUE r_height) {
  Check_Type(r_width, T_FIXNUM);
  Check_Type(r_height, T_FIXNUM);

  cv::Size *size = SG_GET_SIZE(self);

  size->width = FIX2INT(r_width);
  size->height = FIX2INT(r_height);

  return self;
}

VALUE
rb_size_get_area(VALUE self) {
  cv::Size *size = SG_GET_SIZE(self);
  return INT2FIX(size->area());
}

VALUE
rb_size_get_height(VALUE self) {
  cv::Size *size = SG_GET_SIZE(self);
  return INT2FIX(size->height);
}

VALUE
rb_size_set_height(VALUE self, VALUE r_height) {
  Check_Type(r_height, T_FIXNUM);

  cv::Size *size = SG_GET_SIZE(self);

  size->height = FIX2INT(r_height);
  return r_height;
}

VALUE
rb_size_get_width(VALUE self) {
  cv::Size *size = SG_GET_SIZE(self);
  return INT2FIX(size->width);
}

VALUE
rb_size_set_width(VALUE self, VALUE r_width) {
  Check_Type(r_width, T_FIXNUM);

  cv::Size *size = SG_GET_SIZE(self);

  size->width = FIX2INT(r_width);
  return r_width;
}

VALUE
cv_img_to_rb_size(cv::Mat *mat) {
  cv::Size *size = new cv::Size(mat->size());
  return Data_Wrap_Struct(rb_cSize, NULL, rb_size_free, size);
}

VALUE
cv_rect_to_rb_size(cv::Rect *rect) {
  cv::Size *size = new cv::Size(rect->size());
  return Data_Wrap_Struct(rb_cSize, NULL, rb_size_free, size);
}
