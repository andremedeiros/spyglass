#ifndef SPYGLASS_BACKGROUND_SUBTRACTOR_H_
#define SPYGLASS_BACKGROUND_SUBTRACTOR_H_

#include "spyglass.h"

#include "bgslib_dp_prati_mediod_bgs.h"

void rb_bgs_init();
void rb_bgs_free(cv::BackgroundSubtractor *bg);
VALUE rb_bgs_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_bgs_subtract(int argc, VALUE *argv, VALUE self);

// BGS Allocators
VALUE rb_bgs_mog_alloc(VALUE self);
VALUE rb_bgs_mog2_alloc(VALUE self);
VALUE rb_bgs_gmg_alloc(VALUE self);
VALUE rb_bgs_prati_mediod_alloc(VALUE self);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_BG_SUBTRACTOR, cv::BackgroundSubtractor);

#endif // SPYGLASS_BACKGROUND_SUBTRACTOR_H_
