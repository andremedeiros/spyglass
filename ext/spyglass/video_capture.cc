#include "video_capture.h"

static VALUE VideoCaptureClass;

namespace Spyglass {
  namespace VideoCapture {
    void define_ruby_class() {
      VideoCaptureClass = rb_define_class_under(Spyglass::get_ruby_module(), "VideoCapture", rb_cObject);
      rb_define_alloc_func(VideoCaptureClass, rb_alloc);
      rb_define_method(VideoCaptureClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 1);

      // Instance methods
      rb_define_method(VideoCaptureClass, ">>", RUBY_METHOD_FUNC(rb_capture), 1);
      rb_define_method(VideoCaptureClass, "open?", RUBY_METHOD_FUNC(rb_is_open), 0);
      rb_define_method(VideoCaptureClass, "stop", RUBY_METHOD_FUNC(rb_stop), 0);
    }

    VALUE get_ruby_class() {
      return VideoCaptureClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::VideoCapture *cap = new cv::VideoCapture();
      return Data_Wrap_Struct(VideoCaptureClass, NULL, rb_free, cap);
    }

    static void rb_free(cv::VideoCapture *cap) {
      if(cap->isOpened())
        cap->release();

      cap->~VideoCapture();
      delete cap;
    }

    static VALUE rb_initialize(VALUE self, VALUE src) {
      cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);

      int type = TYPE(src);

      switch(type) {
        case T_FIXNUM:
          cap->open(FIX2INT(src));
          break;
        case T_STRING:
          cap->open(StringValueCStr(src));
          break;
        default:
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum or String)",
              rb_obj_classname(src));
      }

      return self;
    }

    static VALUE rb_capture(VALUE self, VALUE dest) {
      if(CLASS_OF(dest) != Spyglass::Image::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
            rb_obj_classname(dest));
      }

      cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);
      cv::Mat *img          = SG_GET_IMAGE(dest);

      (*cap) >> (*img);

      return dest;
    }

    static VALUE rb_is_open(VALUE self) {
      cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);
      return (cap->isOpened()) ? Qtrue : Qfalse;
    }

    static VALUE rb_stop(VALUE self) {
      cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);
      cap->release();
      return self;
    }
  }
}
