#include "image.h"

static VALUE ImageClass;

namespace OpenCV {
  namespace Image {
    void define_ruby_class() {
      // Class definition
      ImageClass = rb_define_class_under(OpenCV::get_ruby_module(), "Image", rb_cObject);
      rb_define_alloc_func(ImageClass, rb_opencv_image_alloc);
      rb_define_method(ImageClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_image_initialize), 0);

      // Class methods
      rb_define_singleton_method(ImageClass, "load", RUBY_METHOD_FUNC(rb_opencv_image_load), -1);

      // Instance methods
      rb_define_method(ImageClass, "cols", RUBY_METHOD_FUNC(rb_opencv_image_get_cols), 0);
      rb_define_method(ImageClass, "crop", RUBY_METHOD_FUNC(rb_opencv_image_crop), 1);
      rb_define_method(ImageClass, "crop!", RUBY_METHOD_FUNC(rb_opencv_image_crop_inplace), 1);
      rb_define_method(ImageClass, "dilate", RUBY_METHOD_FUNC(rb_opencv_image_dilate), -1);
      rb_define_method(ImageClass, "dilate!", RUBY_METHOD_FUNC(rb_opencv_image_dilate_inplace), -1);
      rb_define_method(ImageClass, "erode", RUBY_METHOD_FUNC(rb_opencv_image_erode), -1);
      rb_define_method(ImageClass, "erode!", RUBY_METHOD_FUNC(rb_opencv_image_erode_inplace), -1);
      rb_define_method(ImageClass, "rows", RUBY_METHOD_FUNC(rb_opencv_image_get_rows), 0);
      rb_define_method(ImageClass, "size", RUBY_METHOD_FUNC(rb_opencv_image_get_size), 0);
      rb_define_method(ImageClass, "write", RUBY_METHOD_FUNC(rb_opencv_image_write), 1);
    }

    VALUE get_ruby_class() {
      return ImageClass;
    }

    VALUE rb_opencv_image_alloc(VALUE self) {
      cv::Mat *mat = new cv::Mat();
      return Data_Wrap_Struct(ImageClass, NULL, rb_opencv_image_free, mat);
    }

    void rb_opencv_image_free(cv::Mat *img) {
      img->~Mat();
      delete img;
    }

    VALUE rb_opencv_image_initialize(VALUE self) {
      return self;
    }


    VALUE rb_opencv_image_dilate(int argc, VALUE *argv, VALUE self) {
      VALUE iterations;
      rb_scan_args(argc, argv, "01", &iterations);

      if(RTEST(iterations) && TYPE(iterations) != T_FIXNUM) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum)", rb_obj_classname(iterations));
      }

      int iter = RTEST(iterations) ? FIX2INT(iterations) : 1;

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);

      cv::Mat *new_img = new cv::Mat();
      cv::dilate(*img, *new_img, cv::Mat(), cv::Point(-1, -1), iter);
      return Data_Wrap_Struct(ImageClass, NULL, rb_opencv_image_free, new_img);
    }

    VALUE rb_opencv_image_dilate_inplace(int argc, VALUE *argv, VALUE self) {
      VALUE iterations;
      rb_scan_args(argc, argv, "01", &iterations);

      if(RTEST(iterations) && TYPE(iterations) != T_FIXNUM) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum)", rb_obj_classname(iterations));
      }

      int iter = RTEST(iterations) ? FIX2INT(iterations) : 1;

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);

      cv::dilate(*img, *img, cv::Mat(), cv::Point(-1, -1), iter);
      return self;
    }

    VALUE rb_opencv_image_erode(int argc, VALUE *argv, VALUE self) {
      VALUE iterations;
      rb_scan_args(argc, argv, "01", &iterations);

      if(RTEST(iterations) && TYPE(iterations) != T_FIXNUM) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum)", rb_obj_classname(iterations));
      }

      int iter = RTEST(iterations) ? FIX2INT(iterations) : 1;

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);

      cv::Mat *new_img = new cv::Mat();
      cv::erode(*img, *new_img, cv::Mat(), cv::Point(-1, -1), iter);
      return Data_Wrap_Struct(ImageClass, NULL, rb_opencv_image_free, new_img);
    }

    VALUE rb_opencv_image_erode_inplace(int argc, VALUE *argv, VALUE self) {
      VALUE iterations;
      rb_scan_args(argc, argv, "01", &iterations);

      if(RTEST(iterations) && TYPE(iterations) != T_FIXNUM) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum)", rb_obj_classname(iterations));
      }

      int iter = RTEST(iterations) ? FIX2INT(iterations) : 1;

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);

      cv::erode(*img, *img, cv::Mat(), cv::Point(-1, -1), iter);
      return self;
    }

    VALUE rb_opencv_image_load(int argc, VALUE *argv, VALUE klass) {
      VALUE filename;
      rb_scan_args(argc, argv, "10", &filename);
      Check_Type(filename, T_STRING);

      // TODO: Find a better way to solve this.
      cv::Mat _img = cv::imread(StringValueCStr(filename));
      cv::Mat *img = new cv::Mat(_img);
      return Data_Wrap_Struct(ImageClass, NULL, rb_opencv_image_free, img);
    }

    VALUE rb_opencv_image_get_cols(VALUE self) {
      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);
      return INT2FIX(img->cols);
    }

    VALUE rb_opencv_image_crop(VALUE self, VALUE rect) {
      if(!CLASS_OF(rect) == OpenCV::Rect::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected OpenCV::Rect)", rb_obj_classname(rect));
      }

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);

      cv::Rect *boundaries;
      Data_Get_Struct(rect, cv::Rect, boundaries);

      cv::Mat *res = new cv::Mat((*img)(*boundaries));
      return Data_Wrap_Struct(ImageClass, NULL, rb_opencv_image_free, res);
    }

    VALUE rb_opencv_image_crop_inplace(VALUE self, VALUE rect) {
      if(!CLASS_OF(rect) == OpenCV::Rect::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected OpenCV::Rect)", rb_obj_classname(rect));
      }

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);

      cv::Rect *boundaries;
      Data_Get_Struct(rect, cv::Rect, boundaries);

      cv::Mat *new_img = new cv::Mat();
      (*img)(*boundaries).copyTo(*new_img);
      rb_opencv_image_free(img);

      Data_Set_Struct(self, new_img);
      return self;
    }

    VALUE rb_opencv_image_get_rows(VALUE self) {
      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);
      return INT2FIX(img->rows);
    }

    VALUE rb_opencv_image_get_size(VALUE self) {
      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);
      return OpenCV::Size::rb_opencv_size_from_cvmat(img);
    }

    VALUE rb_opencv_image_write(VALUE self, VALUE filename) {
      Check_Type(filename, T_STRING);

      cv::Mat *img;
      Data_Get_Struct(self, cv::Mat, img);
      bool res = cv::imwrite(StringValueCStr(filename), *img);
      return (res) ? Qtrue : Qfalse;
    }
  }
}
