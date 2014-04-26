#ifndef SPYGLASS_RECT_H_
#define SPYGLASS_RECT_H_

#include "spyglass.h"

void rb_rect_init();

VALUE rb_rect_alloc(VALUE self);
void rb_rect_free(cv::Rect *rect);
VALUE rb_rect_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height);
VALUE rb_rect_contains(VALUE self, VALUE other);
VALUE rb_rect_get_area(VALUE self);
VALUE rb_rect_get_center(VALUE self);
VALUE rb_rect_get_height(VALUE self);
VALUE rb_rect_get_point(VALUE self);
VALUE rb_rect_get_size(VALUE self);
VALUE rb_rect_get_width(VALUE self);
VALUE rb_rect_get_x(VALUE self);
VALUE rb_rect_get_y(VALUE self);
VALUE rb_rect_set_height(VALUE self, VALUE height);
VALUE rb_rect_set_width(VALUE self, VALUE width);
VALUE rb_rect_set_x(VALUE self, VALUE x);
VALUE rb_rect_set_y(VALUE self, VALUE y);
VALUE rb_rect_to_a(VALUE self);

VALUE cv_rect_to_rb_rect(cv::Rect *rect);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_RECT, cv::Rect);

#endif // SPYGLASS_RECT_H_
