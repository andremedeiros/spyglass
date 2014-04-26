#ifndef SPYGLASS_CONTOUR_H_
#define SPYGLASS_CONTOUR_H_

#include "spyglass.h"

void rb_contour_init();

VALUE rb_contour_alloc(VALUE self);
void rb_contour_free(std::vector<cv::Point *> *contour);
VALUE rb_contour_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_contour_get_corners(VALUE self);
VALUE rb_contour_get_rect(VALUE self);
VALUE rb_contour_is_convex(VALUE self);

std::vector<cv::Point> cv_contour_to_cv_points(std::vector<cv::Point *> *contour);
VALUE cv_points_to_rb_vector(std::vector<cv::Point> contours);
VALUE cv_contours_to_rb_contours(std::vector<std::vector<cv::Point> > contours);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_CONTOUR, std::vector<cv::Point *>);

#endif // SPYGLASS_CONTOUR_H_
