#include "color.h"

extern VALUE rb_mSpyglass;

VALUE rb_cColor = Qnil;

void
rb_color_init() {
  // Class definition
  rb_cColor = rb_define_class_under(rb_mSpyglass, "Color", rb_cObject);
  rb_define_alloc_func(rb_cColor, rb_color_alloc);
  rb_define_method(rb_cColor, "initialize", RUBY_METHOD_FUNC(rb_color_initialize), -1);

  // Instance methods
  rb_define_method(rb_cColor, "[]", RUBY_METHOD_FUNC(rb_color_get_component), 1);
  rb_define_method(rb_cColor, "[]=", RUBY_METHOD_FUNC(rb_color_set_component), 2);
  rb_define_method(rb_cColor, "to_a", RUBY_METHOD_FUNC(rb_color_to_a), 0);
  rb_define_method(rb_cColor, "zeros?", RUBY_METHOD_FUNC(rb_color_is_zeros), 0);
}

VALUE
rb_color_alloc(VALUE klass) {
  cv::Scalar *color = new cv::Scalar(0, 0, 0, 0);
  return TypedData_Wrap_Struct(rb_cColor, &color_data_type, color);
}

void
rb_color_free(void *cobj) {
  cv::Scalar *color = (cv::Scalar *)cobj;
  delete color;
}

size_t
rb_color_memsize(const void *cobj) {
  return sizeof(cv::Scalar);
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
rb_color_get_component(VALUE self, VALUE r_index) {
  Check_Type(r_index, T_FIXNUM);

  int idx = NUM2INT(r_index);

  if(idx < 0 || idx > 3)
    return Qnil;

  cv::Scalar *color = SG_GET_COLOR(self);
  double val = (*color)[idx];
  return DBL2NUM(val);
}

VALUE
rb_color_set_component(VALUE self, VALUE r_index, VALUE r_value) {
  cv::Scalar *color = SG_GET_COLOR(self);
  int idx = NUM2INT(r_index);

  color[idx] = NUM2DBL(r_value);

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
  return TypedData_Wrap_Struct(rb_cColor, &color_data_type, color);
}
