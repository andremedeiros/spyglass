#ifndef SPYGLASS_VIDEO_CAPTURE_H_
#define SPYGLASS_VIDEO_CAPTURE_H_

#include "spyglass.h"

void rb_cap_init();
VALUE rb_cap_alloc(VALUE self);
void rb_cap_free(cv::VideoCapture *cap);
VALUE rb_cap_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_cap_capture(VALUE self, VALUE r_dest);
VALUE rb_cap_is_open(VALUE self);
VALUE rb_cap_stop(VALUE self);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_VIDEO_CAPTURE, cv::VideoCapture);

#endif // SPYGLASS_VIDEO_CAPTURE_H_
