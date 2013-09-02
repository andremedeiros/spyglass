#ifndef SPYGLASS_BACKGROUND_SUBTRACTOR_H_
#define SPYGLASS_BACKGROUND_SUBTRACTOR_H_

#include "spyglass.h"

namespace Spyglass {
  namespace BackgroundSubtractor {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::BackgroundSubtractorMOG2 *bg);
    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
    static VALUE rb_subtract(int argc, VALUE *argv, VALUE self);
  }
}

#endif // SPYGLASS_BACKGROUND_SUBTRACTOR_H_
