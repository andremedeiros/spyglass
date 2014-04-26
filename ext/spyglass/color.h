#ifndef SPYGLASS_COLOR_H_
#define SPYGLASS_COLOR_H_

#include "spyglass.h"

void rb_color_init();
VALUE rb_color_alloc(VALUE self);
void rb_color_free(cv::Scalar *color);
VALUE rb_color_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_color_get_component(VALUE self, VALUE r_index);
VALUE rb_color_set_component(VALUE self, VALUE r_index, VALUE r_color);
VALUE rb_color_is_zeros(VALUE self);
VALUE rb_color_to_a(VALUE self);

VALUE cv_color_to_rb_color(cv::Scalar *color);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_COLOR, cv::Scalar);

#endif // SPYGLASS_COLOR_H_
