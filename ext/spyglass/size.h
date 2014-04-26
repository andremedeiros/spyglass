#ifndef SPYGLASS_SIZE_H_
#define SPYGLASS_SIZE_H_

#include "spyglass.h"

void rb_size_init();
VALUE rb_size_alloc(VALUE self);
void rb_size_free(cv::Size *size);
VALUE rb_size_initialize(VALUE self, VALUE width, VALUE height);
VALUE rb_size_get_area(VALUE self);
VALUE rb_size_get_width(VALUE self);
VALUE rb_size_get_height(VALUE self);
VALUE rb_size_set_width(VALUE self, VALUE width);
VALUE rb_size_set_height(VALUE self, VALUE height);

VALUE cv_img_to_rb_size(cv::Mat *mat);
VALUE cv_rect_to_rb_size(cv::Rect *rect);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_SIZE, cv::Size);

#endif // SPYGLASS_SIZE_H_
