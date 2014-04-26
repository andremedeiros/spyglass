#include "color.h"

extern VALUE SpyglassModule;

VALUE ColorClass = Qnil;

void
rb_color_init() {
  // Class definition
  ColorClass = rb_define_class_under(SpyglassModule, "Color", rb_cObject);
  rb_define_alloc_func(ColorClass, rb_color_alloc);
  rb_define_method(ColorClass, "initialize", RUBY_METHOD_FUNC(rb_color_initialize), -1);

  // Instance methods
  rb_define_method(ColorClass, "[]", RUBY_METHOD_FUNC(rb_color_get_component), 1);
  rb_define_method(ColorClass, "[]=", RUBY_METHOD_FUNC(rb_color_set_component), 2);
  rb_define_method(ColorClass, "to_a", RUBY_METHOD_FUNC(rb_color_to_a), 0);
  rb_define_method(ColorClass, "zeros?", RUBY_METHOD_FUNC(rb_color_is_zeros), 0);
}

VALUE
rb_color_alloc(VALUE self) {
  cv::Scalar *color = new cv::Scalar(0, 0, 0, 0);
  return Data_Wrap_Struct(ColorClass, NULL, rb_color_free, color);
}

void
rb_color_free(cv::Scalar *color) {
  delete color;
}

VALUE
rb_color_initialize(int argc, VALUE *argv, VALUE self) {
  if(argc < 1 || argc > 4)
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..4)", argc);

  cv::Scalar *color = SG_GET_COLOR(self);

  for(int idx = 0; idx < argc; idx++) {
    (*color)[idx] = NUM2DBL(argv[idx]);
  }

  return self;
}

VALUE
rb_color_get_component(VALUE self, VALUE index) {
  Check_Type(index, T_FIXNUM);

  int idx = NUM2INT(index);

  if(idx < 0 || idx > 3)
    return Qnil;

  cv::Scalar *color = SG_GET_COLOR(self);
  double val = (*color)[idx];
  return DBL2NUM(val);
}

VALUE
rb_color_set_component(VALUE self, VALUE index, VALUE value) {
  cv::Scalar *color = SG_GET_COLOR(self);
  int idx = NUM2INT(index);

  color[idx] = NUM2DBL(value);

  return self;
}

VALUE
rb_color_to_a(VALUE self) {
  cv::Scalar *color = SG_GET_COLOR(self);
  VALUE ary = rb_ary_new2(4);

  for(int idx = 0; idx < 4; idx++)
    rb_ary_store(ary, idx, DBL2NUM((*color)[idx]));

  return ary;
}

VALUE
rb_color_is_zeros(VALUE self) {
  cv::Scalar *color = SG_GET_COLOR(self);

  for(int idx = 0; idx < 4; idx++)
    if((*color)[idx] > 0) return Qfalse;

  return Qtrue;
}

VALUE
cv_color_to_rb_color(cv::Scalar *color) {
  return Data_Wrap_Struct(ColorClass, NULL, rb_color_free, color);
}
