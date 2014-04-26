#include "size.h"

extern VALUE SpyglassModule;

VALUE SizeClass = Qnil;

void
rb_size_init() {
  // Class definition
  SizeClass = rb_define_class_under(SpyglassModule, "Size", rb_cObject);
  rb_define_alloc_func(SizeClass, rb_size_alloc);
  rb_define_method(SizeClass, "initialize", RUBY_METHOD_FUNC(rb_size_initialize), 2);

  // Instance methods
  rb_define_method(SizeClass, "area", RUBY_METHOD_FUNC(rb_size_get_area), 0);
  rb_define_method(SizeClass, "height", RUBY_METHOD_FUNC(rb_size_get_height), 0);
  rb_define_method(SizeClass, "height=", RUBY_METHOD_FUNC(rb_size_set_height), 1);
  rb_define_method(SizeClass, "width", RUBY_METHOD_FUNC(rb_size_get_width), 0);
  rb_define_method(SizeClass, "width=", RUBY_METHOD_FUNC(rb_size_set_width), 1);
}

VALUE
rb_size_alloc(VALUE self) {
  cv::Size *size = new cv::Size();
  return Data_Wrap_Struct(SizeClass, NULL, rb_size_free, size);
}

void
rb_size_free(cv::Size *size) {
  delete size;
}

VALUE
rb_size_initialize(VALUE self, VALUE width, VALUE height) {
  Check_Type(width, T_FIXNUM);
  Check_Type(height, T_FIXNUM);

  cv::Size *size = SG_GET_SIZE(self);

  size->width = FIX2INT(width);
  size->height = FIX2INT(height);

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
rb_size_set_height(VALUE self, VALUE height) {
  Check_Type(height, T_FIXNUM);

  cv::Size *size = SG_GET_SIZE(self);

  size->height = FIX2INT(height);
  return height;
}

VALUE
rb_size_get_width(VALUE self) {
  cv::Size *size = SG_GET_SIZE(self);
  return INT2FIX(size->width);
}

VALUE
rb_size_set_width(VALUE self, VALUE width) {
  Check_Type(width, T_FIXNUM);

  cv::Size *size = SG_GET_SIZE(self);

  size->width = FIX2INT(width);
  return width;
}

VALUE
cv_img_to_rb_size(cv::Mat *mat) {
  cv::Size *size = new cv::Size(mat->size());
  return Data_Wrap_Struct(SizeClass, NULL, rb_size_free, size);
}

VALUE
cv_rect_to_rb_size(cv::Rect *rect) {
  cv::Size *size = new cv::Size(rect->size());
  return Data_Wrap_Struct(SizeClass, NULL, rb_size_free, size);
}
