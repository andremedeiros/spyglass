#include "size.h"

static VALUE SizeClass;

namespace OpenCV {
  namespace Size {
    void define_ruby_class() {
      VALUE opencv_module = rb_opencv_module();

      // Class definition
      SizeClass = rb_define_class_under(opencv_module, "Size", rb_cObject);
      rb_define_alloc_func(SizeClass, rb_opencv_size_alloc);
      rb_define_method(SizeClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_size_initialize), 2);

      // Instance methods
      rb_define_method(SizeClass, "area", RUBY_METHOD_FUNC(rb_opencv_size_get_area), 0);
      rb_define_method(SizeClass, "height", RUBY_METHOD_FUNC(rb_opencv_size_get_height), 0);
      rb_define_method(SizeClass, "height=", RUBY_METHOD_FUNC(rb_opencv_size_set_height), 1);
      rb_define_method(SizeClass, "width", RUBY_METHOD_FUNC(rb_opencv_size_get_width), 0);
      rb_define_method(SizeClass, "width=", RUBY_METHOD_FUNC(rb_opencv_size_set_width), 1);
    }

    VALUE get_ruby_class() {
      return SizeClass;
    }

    VALUE rb_opencv_size_alloc(VALUE self) {
      cv::Size *size = new cv::Size();
      return Data_Wrap_Struct(SizeClass, NULL, rb_opencv_size_free, size);
    }

    void rb_opencv_size_free(cv::Size *size) {
      delete size;
    }

    VALUE rb_opencv_size_initialize(VALUE self, VALUE width, VALUE height) {
      Check_Type(width, T_FIXNUM);
      Check_Type(height, T_FIXNUM);

      cv::Size *size;
      Data_Get_Struct(self, cv::Size, size);

      size->width = FIX2INT(width);
      size->height = FIX2INT(height);

      return self;
    }

    VALUE rb_opencv_size_get_area(VALUE self) {
      cv::Size *size;
      Data_Get_Struct(self, cv::Size, size);
      return INT2FIX(size->area());
    }

    VALUE rb_opencv_size_get_height(VALUE self) {
      cv::Size *size;
      Data_Get_Struct(self, cv::Size, size);
      return INT2FIX(size->height);
    }

    VALUE rb_opencv_size_set_height(VALUE self, VALUE height) {
      Check_Type(height, T_FIXNUM);

      cv::Size *size;
      Data_Get_Struct(self, cv::Size, size);

      size->height = FIX2INT(height);
      return height;
    }

    VALUE rb_opencv_size_get_width(VALUE self) {
      cv::Size *size;
      Data_Get_Struct(self, cv::Size, size);
      return INT2FIX(size->width);
    }

    VALUE rb_opencv_size_set_width(VALUE self, VALUE width) {
      Check_Type(width, T_FIXNUM);

      cv::Size *size;
      Data_Get_Struct(self, cv::Size, size);

      size->width = FIX2INT(width);
      return width;
    }

    VALUE rb_opencv_size_from_cvmat(cv::Mat *mat) {
      cv::Size *size = new cv::Size(mat->size());
      return Data_Wrap_Struct(SizeClass, NULL, rb_opencv_size_free, size);
    }

    VALUE rb_opencv_size_from_cvrect(cv::Rect *rect) {
      cv::Size *size = new cv::Size(rect->size());
      return Data_Wrap_Struct(SizeClass, NULL, rb_opencv_size_free, size);
    }
  }
}
