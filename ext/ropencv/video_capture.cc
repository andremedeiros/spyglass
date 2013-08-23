#include "video_capture.h"

static VALUE VideoCaptureClass;

namespace OpenCV {
  namespace VideoCapture {
    void define_ruby_class() {
      VideoCaptureClass = rb_define_class_under(OpenCV::get_ruby_module(), "VideoCapture", rb_cObject);
      rb_define_alloc_func(VideoCaptureClass, rb_opencv_video_capture_alloc);
      rb_define_method(VideoCaptureClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_video_capture_initialize), 1);

      // Instance methods
      rb_define_method(VideoCaptureClass, ">>", RUBY_METHOD_FUNC(rb_opencv_video_capture_capture), 1);
      rb_define_method(VideoCaptureClass, "open?", RUBY_METHOD_FUNC(rb_opencv_video_capture_get_open), 0);
      rb_define_method(VideoCaptureClass, "stop", RUBY_METHOD_FUNC(rb_opencv_video_capture_stop), 0);
    }

    VALUE get_ruby_class() {
      return VideoCaptureClass;
    }

    VALUE rb_opencv_video_capture_alloc(VALUE self) {
      cv::VideoCapture *cap = new cv::VideoCapture();
      return Data_Wrap_Struct(VideoCaptureClass, NULL, rb_opencv_video_capture_free, cap);
    }

    void rb_opencv_video_capture_free(cv::VideoCapture *cap) {
      if(cap->isOpened())
        cap->release();

      cap->~VideoCapture();
      delete cap;
    }

    VALUE rb_opencv_video_capture_initialize(VALUE self, VALUE src) {
      cv::VideoCapture *cap;
      Data_Get_Struct(self, cv::VideoCapture, cap);

      int type = TYPE(src);

      switch(type) {
        case T_FIXNUM:
          cap->open(FIX2INT(src));
          break;
        case T_STRING:
          cap->open(StringValueCStr(src));
          break;
        default:
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum or String)", rb_obj_classname(src));
      }

      return self;
    }

    VALUE rb_opencv_video_capture_capture(VALUE self, VALUE dest) {
      if(CLASS_OF(dest) != OpenCV::Image::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected OpenCV::Image)", rb_obj_classname(dest));
      }

      cv::VideoCapture *cap;
      Data_Get_Struct(self, cv::VideoCapture, cap);

      cv::Mat *img;
      Data_Get_Struct(dest, cv::Mat, img);

      (*cap) >> (*img);

      return dest;
    }

    VALUE rb_opencv_video_capture_get_open(VALUE self) {
      cv::VideoCapture *cap;
      Data_Get_Struct(self, cv::VideoCapture, cap);

      return (cap->isOpened()) ? Qtrue : Qfalse;
    }

    VALUE rb_opencv_video_capture_stop(VALUE self) {
      cv::VideoCapture *cap;
      Data_Get_Struct(self, cv::VideoCapture, cap);

      cap->release();

      return self;
    }
  }
}
