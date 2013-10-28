#include "color.h"

static VALUE ColorClass;

namespace Spyglass {
  namespace Color {
    void define_ruby_class() {
      // Class definition
      ColorClass = rb_define_class_under(Spyglass::get_ruby_module(), "Color", rb_cObject);
      rb_define_alloc_func(ColorClass, rb_alloc);
      rb_define_method(ColorClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);

      // Instance methods
      rb_define_method(ColorClass, "[]", RUBY_METHOD_FUNC(rb_get_color), 1);
      rb_define_method(ColorClass, "[]=", RUBY_METHOD_FUNC(rb_set_color), 2);
      rb_define_method(ColorClass, "to_a", RUBY_METHOD_FUNC(rb_to_a), 0);
      rb_define_method(ColorClass, "zeros?", RUBY_METHOD_FUNC(rb_is_zeros), 0);
    }

    VALUE get_ruby_class() {
      return ColorClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::Scalar *color = new cv::Scalar(0, 0, 0, 0);
      return Data_Wrap_Struct(ColorClass, NULL, rb_free, color);
    }

    static void rb_free(cv::Scalar *color) {
      delete color;
    }

    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self) {
      if(argc < 1 || argc > 4)
        rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..4)", argc);

      cv::Scalar *color = SG_GET_COLOR(self);

      for(int idx = 0; idx < argc; idx++) {
        (*color)[idx] = NUM2DBL(argv[idx]);
      }

      return self;
    }

    static VALUE rb_get_color(VALUE self, VALUE index) {
      Check_Type(index, T_FIXNUM);

      int idx = NUM2INT(index);

      if(idx < 0 || idx > 3)
        return Qnil;

      cv::Scalar *color = SG_GET_COLOR(self);
      double val = (*color)[idx];
      return DBL2NUM(val);
    }

    static VALUE rb_set_color(VALUE self, VALUE index, VALUE value) {
      cv::Scalar *color = SG_GET_COLOR(self);
      int idx = NUM2INT(index);

      color[idx] = NUM2DBL(value);

      return self;
    }

    static VALUE rb_to_a(VALUE self) {
      cv::Scalar *color = SG_GET_COLOR(self);
      VALUE ary = rb_ary_new2(4);

      for(int idx = 0; idx < 4; idx++)
        rb_ary_store(ary, idx, DBL2NUM((*color)[idx]));

      return ary;
    }

    static VALUE rb_is_zeros(VALUE self) {
      cv::Scalar *color = SG_GET_COLOR(self);
      bool all_zeros = true;
      for(int idx = 0; idx < 4; idx++) {
        if((*color)[idx] > 0)
          all_zeros = false;
      }

      return all_zeros ? Qtrue : Qfalse;
    }

    VALUE from_cvscalar(cv::Scalar *color) {
      return Data_Wrap_Struct(ColorClass, NULL, rb_free, color);
    }
  }
}
