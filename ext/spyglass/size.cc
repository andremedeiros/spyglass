#include "size.h"

static VALUE SizeClass;

namespace Spyglass {
  namespace Size {
    void define_ruby_class() {
      // Class definition
      SizeClass = rb_define_class_under(Spyglass::get_ruby_module(), "Size", rb_cObject);
      rb_define_alloc_func(SizeClass, rb_alloc);
      rb_define_method(SizeClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 2);

      // Instance methods
      rb_define_method(SizeClass, "area", RUBY_METHOD_FUNC(rb_get_area), 0);
      rb_define_method(SizeClass, "height", RUBY_METHOD_FUNC(rb_get_height), 0);
      rb_define_method(SizeClass, "height=", RUBY_METHOD_FUNC(rb_set_height), 1);
      rb_define_method(SizeClass, "width", RUBY_METHOD_FUNC(rb_get_width), 0);
      rb_define_method(SizeClass, "width=", RUBY_METHOD_FUNC(rb_set_width), 1);
    }

    VALUE get_ruby_class() {
      return SizeClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::Size *size = new cv::Size();
      return Data_Wrap_Struct(SizeClass, NULL, rb_free, size);
    }

    static void rb_free(cv::Size *size) {
      delete size;
    }

    static VALUE rb_initialize(VALUE self, VALUE width, VALUE height) {
      Check_Type(width, T_FIXNUM);
      Check_Type(height, T_FIXNUM);

      cv::Size *size = SG_GET_SIZE(self);

      size->width = FIX2INT(width);
      size->height = FIX2INT(height);

      return self;
    }

    static VALUE rb_get_area(VALUE self) {
      cv::Size *size = SG_GET_SIZE(self);
      return INT2FIX(size->area());
    }

    static VALUE rb_get_height(VALUE self) {
      cv::Size *size = SG_GET_SIZE(self);
      return INT2FIX(size->height);
    }

    static VALUE rb_set_height(VALUE self, VALUE height) {
      Check_Type(height, T_FIXNUM);

      cv::Size *size = SG_GET_SIZE(self);

      size->height = FIX2INT(height);
      return height;
    }

    static VALUE rb_get_width(VALUE self) {
      cv::Size *size = SG_GET_SIZE(self);
      return INT2FIX(size->width);
    }

    static VALUE rb_set_width(VALUE self, VALUE width) {
      Check_Type(width, T_FIXNUM);

      cv::Size *size = SG_GET_SIZE(self);

      size->width = FIX2INT(width);
      return width;
    }

    VALUE from_cvmat(cv::Mat *mat) {
      cv::Size *size = new cv::Size(mat->size());
      return Data_Wrap_Struct(SizeClass, NULL, rb_free, size);
    }

    VALUE from_cvrect(cv::Rect *rect) {
      cv::Size *size = new cv::Size(rect->size());
      return Data_Wrap_Struct(SizeClass, NULL, rb_free, size);
    }
  }
}
