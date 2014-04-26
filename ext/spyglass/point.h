#ifndef SPYGLASS_POINT_H_
#define SPYGLASS_POINT_H_

#include "spyglass.h"

void rb_point_init();
VALUE rb_point_alloc(VALUE self);
void rb_point_free(cv::Point *point);
VALUE rb_point_initialize(VALUE self, VALUE x, VALUE y);
VALUE rb_point_get_x(VALUE self);
VALUE rb_point_get_y(VALUE self);
VALUE rb_point_set_x(VALUE self, VALUE x);
VALUE rb_point_set_y(VALUE self, VALUE y);

VALUE cv_point_to_rb_point(cv::Point *point);
VALUE cv_rect_to_rb_point(cv::Rect *rect);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_POINT, cv::Point);

#endif // SPYGLASS_POINT_H_
