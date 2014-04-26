#ifndef SPYGLASS_IMAGE_H_
#define SPYGLASS_IMAGE_H_

#include "spyglass.h"

void rb_img_init();
VALUE rb_img_alloc(VALUE self);
void rb_img_free(cv::Mat *mat);
VALUE rb_img_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_img_canny(VALUE self, VALUE threshold1, VALUE threshold2);
VALUE rb_img_canny_inplace(VALUE self, VALUE threshold1, VALUE threshold2);
VALUE rb_img_convert(VALUE self, VALUE color_space);
VALUE rb_img_convert_inplace(VALUE self, VALUE color_space);
VALUE rb_img_copy_inplace(int argc, VALUE *argv, VALUE self);
VALUE rb_img_crop(VALUE self, VALUE rect);
VALUE rb_img_crop_inplace(VALUE self, VALUE rect);
VALUE rb_img_dilate(int argc, VALUE *argv, VALUE self);
VALUE rb_img_dilate_inplace(int argc, VALUE *argv, VALUE self);
VALUE rb_img_draw_contours(VALUE self, VALUE contours, VALUE color);
VALUE rb_img_draw_rectangle(VALUE self, VALUE rect, VALUE color);
VALUE rb_img_draw_label(VALUE self, VALUE string, VALUE point);
VALUE rb_img_erode(int argc, VALUE *argv, VALUE self);
VALUE rb_img_erode_inplace(int argc, VALUE *argv, VALUE self);
VALUE rb_img_fill(int argc, VALUE *argv, VALUE self);
VALUE rb_img_fill_inplace(int argc, VALUE *argv, VALUE self);
VALUE rb_img_load(int argc, VALUE *argv, VALUE klass);
VALUE rb_img_mean(int argc, VALUE *argv, VALUE klass);
VALUE rb_img_resize(VALUE self, VALUE size);
VALUE rb_img_resize_inplace(VALUE self, VALUE size);
VALUE rb_img_threshold(int argc, VALUE *argv, VALUE klass);
VALUE rb_img_threshold_inplace(int argc, VALUE *argv, VALUE klass);
VALUE rb_img_threshold_inv(int argc, VALUE *argv, VALUE klass);
VALUE rb_img_threshold_inv_inplace(int argc, VALUE *argv, VALUE klass);
VALUE rb_img_get_color_at(VALUE self, VALUE point);
VALUE rb_img_get_cols(VALUE self);
VALUE rb_img_get_contours(VALUE self);
VALUE rb_img_get_rows(VALUE self);
VALUE rb_img_get_size(VALUE self);
VALUE rb_img_warp_perspective(VALUE self, VALUE corners, VALUE size);
VALUE rb_img_write(VALUE self, VALUE filename);
VALUE rb_img_zeros(int argc, VALUE *argv, VALUE klass);

VALUE cv_img_to_rb_img(cv::Mat *mat);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_IMAGE, cv::Mat);

#endif // SPYGLASS_IMAGE_H_
