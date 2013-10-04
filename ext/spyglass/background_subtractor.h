#ifndef SPYGLASS_BACKGROUND_SUBTRACTOR_H_
#define SPYGLASS_BACKGROUND_SUBTRACTOR_H_

#include "spyglass.h"

#include "bgslib_dp_prati_mediod_bgs.h"

namespace Spyglass {
  namespace BackgroundSubtractor {
    void define_ruby_types();
    VALUE get_ruby_module();

    static void rb_free(cv::BackgroundSubtractor *bg);
    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
    static VALUE rb_subtract(int argc, VALUE *argv, VALUE self);

    // Class allocators
    static VALUE rb_mog_alloc(VALUE self);
    static VALUE rb_mog2_alloc(VALUE self);
    static VALUE rb_gmg_alloc(VALUE self);
    static VALUE rb_prati_mediod_alloc(VALUE self);
  }
}

#endif // SPYGLASS_BACKGROUND_SUBTRACTOR_H_
