#ifndef SPYGLASS_VIDEO_CAPTURE_H_
#define SPYGLASS_VIDEO_CAPTURE_H_

#include "spyglass.h"

namespace Spyglass {
  namespace VideoCapture {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::VideoCapture *cap);
    static VALUE rb_initialize(VALUE self, VALUE src);
    static VALUE rb_capture(VALUE self, VALUE dest);
    static VALUE rb_is_open(VALUE self);
    static VALUE rb_stop(VALUE self);
  }
}

#endif // SPYGLASS_VIDEO_CAPTURE_H_
