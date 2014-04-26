#include "image.h"

extern VALUE rb_mSpyglass;
extern VALUE rb_cSize;
extern VALUE rb_cContour;
extern VALUE rb_cColor;
extern VALUE rb_cRect;

VALUE rb_cImage = Qnil;

void
rb_img_init() {
  // Class definition
  rb_cImage = rb_define_class_under(rb_mSpyglass, "Image", rb_cObject);
  rb_define_alloc_func(rb_cImage, rb_img_alloc);
  rb_define_method(rb_cImage, "initialize", RUBY_METHOD_FUNC(rb_img_initialize), -1);

  // Class methods
  rb_define_singleton_method(rb_cImage, "load", RUBY_METHOD_FUNC(rb_img_load), -1);
  rb_define_singleton_method(rb_cImage, "zeros", RUBY_METHOD_FUNC(rb_img_zeros), -1);

  // Instance methods
  rb_define_method(rb_cImage, "canny", RUBY_METHOD_FUNC(rb_img_canny), 2);
  rb_define_method(rb_cImage, "canny!", RUBY_METHOD_FUNC(rb_img_canny_inplace), 2);
  rb_define_method(rb_cImage, "color_at", RUBY_METHOD_FUNC(rb_img_get_color_at), 1);
  rb_define_method(rb_cImage, "cols", RUBY_METHOD_FUNC(rb_img_get_cols), 0);
  rb_define_method(rb_cImage, "contours", RUBY_METHOD_FUNC(rb_img_get_contours), 0);
  rb_define_method(rb_cImage, "convert", RUBY_METHOD_FUNC(rb_img_convert), 1);
  rb_define_method(rb_cImage, "convert!", RUBY_METHOD_FUNC(rb_img_convert_inplace), 1);
  rb_define_method(rb_cImage, "copy!", RUBY_METHOD_FUNC(rb_img_copy_inplace), -1);
  rb_define_method(rb_cImage, "crop", RUBY_METHOD_FUNC(rb_img_crop), 1);
  rb_define_method(rb_cImage, "crop!", RUBY_METHOD_FUNC(rb_img_crop_inplace), 1);
  rb_define_method(rb_cImage, "dilate", RUBY_METHOD_FUNC(rb_img_dilate), -1);
  rb_define_method(rb_cImage, "dilate!", RUBY_METHOD_FUNC(rb_img_dilate_inplace), -1);
  rb_define_method(rb_cImage, "draw_contours", RUBY_METHOD_FUNC(rb_img_draw_contours), 2);
  rb_define_method(rb_cImage, "draw_rectangle", RUBY_METHOD_FUNC(rb_img_draw_rectangle), 2);
  rb_define_method(rb_cImage, "draw_label", RUBY_METHOD_FUNC(rb_img_draw_label), 2);
  rb_define_method(rb_cImage, "erode", RUBY_METHOD_FUNC(rb_img_erode), -1);
  rb_define_method(rb_cImage, "erode!", RUBY_METHOD_FUNC(rb_img_erode_inplace), -1);
  rb_define_method(rb_cImage, "fill", RUBY_METHOD_FUNC(rb_img_fill), -1);
  rb_define_method(rb_cImage, "fill!", RUBY_METHOD_FUNC(rb_img_fill_inplace), -1);
  rb_define_method(rb_cImage, "mean", RUBY_METHOD_FUNC(rb_img_mean), -1);
  rb_define_method(rb_cImage, "resize", RUBY_METHOD_FUNC(rb_img_resize), 1);
  rb_define_method(rb_cImage, "resize!", RUBY_METHOD_FUNC(rb_img_resize_inplace), 1);
  rb_define_method(rb_cImage, "rows", RUBY_METHOD_FUNC(rb_img_get_rows), 0);
  rb_define_method(rb_cImage, "size", RUBY_METHOD_FUNC(rb_img_get_size), 0);
  rb_define_method(rb_cImage, "threshold", RUBY_METHOD_FUNC(rb_img_threshold), -1);
  rb_define_method(rb_cImage, "threshold!", RUBY_METHOD_FUNC(rb_img_threshold_inplace), -1);
  rb_define_method(rb_cImage, "threshold_inv", RUBY_METHOD_FUNC(rb_img_threshold_inv), -1);
  rb_define_method(rb_cImage, "threshold_inv!", RUBY_METHOD_FUNC(rb_img_threshold_inv_inplace), -1);
  rb_define_method(rb_cImage, "warp_perspective", RUBY_METHOD_FUNC(rb_img_warp_perspective), 2);
  rb_define_method(rb_cImage, "write", RUBY_METHOD_FUNC(rb_img_write), 1);

  // Constants
  rb_define_const(rb_cImage, "TYPE_8UC1", INT2NUM(CV_8UC1));
  rb_define_const(rb_cImage, "TYPE_8UC3", INT2NUM(CV_8UC3));
}

VALUE
rb_img_alloc(VALUE self) {
  cv::Mat *mat = new cv::Mat();
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, mat);
}

void
rb_img_free(cv::Mat *img) {
  img->~Mat();
  delete img;
}

VALUE
rb_img_initialize(int argc, VALUE *argv, VALUE self) {
  if(argc == 0)
    return self;

  VALUE size, type;
  rb_scan_args(argc, argv, "02", &size, &type);

  if(RTEST(size) && CLASS_OF(size) != rb_cSize) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Size)",
        rb_obj_classname(size));
  }

  if(RTEST(type))
    Check_Type(type, T_FIXNUM);

  cv::Mat *img = SG_GET_IMAGE(self);
  rb_img_free(img);

  cv::Size *_size = RTEST(size) ? SG_GET_SIZE(size) : new cv::Size();
  int _type       = RTEST(type) ? NUM2INT(type) : CV_8UC3;

  img = new cv::Mat(*_size, _type);
  Data_Set_Struct(self, img);

  return self;
}

cv::Mat *
_do_canny(VALUE self, VALUE threshold1, VALUE threshold2, bool inplace) {
  Check_Type(threshold1, T_FIXNUM);
  Check_Type(threshold2, T_FIXNUM);

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;

  if(inplace)
    dest = img;
  else
    dest = new cv::Mat();

  cv::Canny(*img, *dest, NUM2INT(threshold1), NUM2INT(threshold2));
  return dest;
}

VALUE
rb_img_canny(VALUE self, VALUE threshold1, VALUE threshold2) {
  cv::Mat *img = _do_canny(self, threshold1, threshold2, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_canny_inplace(VALUE self, VALUE threshold1, VALUE threshold2) {
  _do_canny(self, threshold1, threshold2, true);
  return self;
}

VALUE
rb_img_copy_inplace(int argc, VALUE *argv, VALUE self) {
  VALUE src, mask;
  rb_scan_args(argc, argv, "11", &src, &mask);

  if(CLASS_OF(src) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(src));
  }

  if(RTEST(mask) && CLASS_OF(mask) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(mask));
  }

  cv::Mat *dest = SG_GET_IMAGE(self);

  rb_img_free(dest);
  dest = new cv::Mat();

  cv::Mat *_src = SG_GET_IMAGE(src);

  if(RTEST(mask)) {
    cv::Mat *_mask = SG_GET_IMAGE(mask);
    _src->copyTo(*dest, *_mask);
  } else {
    _src->copyTo(*dest);
  }


  Data_Set_Struct(self, dest);
  return self;
}

cv::Mat *
_do_dilate(int argc, VALUE *argv, VALUE self, bool inplace) {
  VALUE iterations;
  rb_scan_args(argc, argv, "01", &iterations);

  if(RTEST(iterations) && TYPE(iterations) != T_FIXNUM) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum)",
        rb_obj_classname(iterations));
  }

  int iter = RTEST(iterations) ? FIX2INT(iterations) : 1;

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;

  if(inplace)
    dest = img;
  else
    dest = new cv::Mat();

  cv::dilate(*img, *dest, cv::Mat(), cv::Point(-1, -1), iter);
  return dest;
}

VALUE
rb_img_dilate(int argc, VALUE *argv, VALUE self) {
  cv::Mat *img = _do_dilate(argc, argv, self, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_dilate_inplace(int argc, VALUE *argv, VALUE self) {
  _do_dilate(argc, argv, self, true);
  return self;
}

VALUE
rb_img_draw_contours(VALUE self, VALUE contours, VALUE color) {
  if(TYPE(contours) != T_ARRAY && CLASS_OF(contours) != rb_cContour)
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Array or Spyglass::Contour)",
        rb_obj_classname(contours));

  if(CLASS_OF(color) != rb_cColor) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Color)",
        rb_obj_classname(color));
  }

  cv::Mat *img        = SG_GET_IMAGE(self);
  cv::Scalar *_color  = SG_GET_COLOR(color);
  std::vector<std::vector<cv::Point> > ctrs;

  if(TYPE(contours) == T_ARRAY) {
    for(int idx = 0; idx < RARRAY_LEN(contours); idx++) {
      std::vector<cv::Point *> *contour = SG_GET_CONTOUR(rb_ary_entry(contours, idx));
      ctrs.push_back(cv_contour_to_cv_points(contour));
    }
  } else {
    std::vector<cv::Point *> *contour = SG_GET_CONTOUR(contours);
    ctrs.push_back(cv_contour_to_cv_points(contour));
  }

  for(int idx = 0; idx < ctrs.size(); idx++)
    cv::drawContours(*img, ctrs, idx, *_color, CV_FILLED);

  return self;
}

VALUE
rb_img_draw_rectangle(VALUE self, VALUE rect, VALUE color) {
  if(CLASS_OF(rect) != rb_cRect) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Rect)",
        rb_obj_classname(rect));
  }

  cv::Mat *img        = SG_GET_IMAGE(self);
  cv::Rect *_rect     = SG_GET_RECT(rect);
  cv::Scalar *_color  = SG_GET_COLOR(color);

  cv::Point bottom_right(_rect->x + _rect->width, _rect->y + _rect->height);

  cv::rectangle(*img, _rect->tl(), bottom_right, *_color);
  return self;
}

VALUE
rb_img_draw_label(VALUE self, VALUE string, VALUE point) {
  cv::Mat *img  = SG_GET_IMAGE(self);
  cv::Point *pt = SG_GET_POINT(point);
  std::string label(StringValueCStr(string));

  const int fontface = cv::FONT_HERSHEY_SIMPLEX;
  int baseline = 0;
  cv::Size size = cv::getTextSize(label, fontface, 0.4, 1, &baseline);
  cv::rectangle(*img, *pt + cv::Point(0, baseline), *pt + cv::Point(size.width, -size.height), CV_RGB(255,255,255), CV_FILLED);
  cv::putText(*img, label, *pt, fontface, 0.4, CV_RGB(0,0,0), 1, 8);
  return self;
}

cv::Mat *
_do_erode(int argc, VALUE *argv, VALUE self, bool inplace) {
  VALUE iterations;
  rb_scan_args(argc, argv, "01", &iterations);

  if(RTEST(iterations) && TYPE(iterations) != T_FIXNUM) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum)",
        rb_obj_classname(iterations));
  }

  int iter = RTEST(iterations) ? FIX2INT(iterations) : 1;

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;

  if(inplace)
    dest = img;
  else
    dest = new cv::Mat();

  cv::erode(*img, *dest, cv::Mat(), cv::Point(-1, -1), iter);
  return dest;
}

VALUE
rb_img_erode(int argc, VALUE *argv, VALUE self) {
  cv::Mat *img = _do_erode(argc, argv, self, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_erode_inplace(int argc, VALUE *argv, VALUE self) {
  _do_erode(argc, argv, self, true);
  return self;
}

cv::Mat *
_do_fill(int argc, VALUE *argv, VALUE self, bool inplace) {
  VALUE color, mask;
  rb_scan_args(argc, argv, "11", &color, &mask);

  if(CLASS_OF(color) != rb_cColor)
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Color)",
        rb_obj_classname(color));

  if(RTEST(mask) && CLASS_OF(mask) != rb_cImage)
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(mask));

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;

  cv::Scalar *_color = SG_GET_COLOR(color);
  cv::Mat *_mask = RTEST(mask) ? SG_GET_IMAGE(mask) : new cv::Mat();

  if(inplace)
    dest = img;
  else
    dest = new cv::Mat(*img);

  (*dest).setTo(*_color, *_mask);
  return dest;
}

VALUE
rb_img_fill(int argc, VALUE *argv, VALUE self) {
  cv::Mat *img = _do_fill(argc, argv, self, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_fill_inplace(int argc, VALUE *argv, VALUE self) {
  _do_fill(argc, argv, self, true);
  return self;
}

VALUE
rb_img_load(int argc, VALUE *argv, VALUE klass) {
  VALUE filename;
  rb_scan_args(argc, argv, "10", &filename);
  Check_Type(filename, T_STRING);

  cv::Mat _img = cv::imread(StringValueCStr(filename));
  cv::Mat *img = new cv::Mat(_img);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_get_color_at(VALUE self, VALUE point) {
  cv::Mat *img      = SG_GET_IMAGE(self);
  cv::Point *coords = SG_GET_POINT(point);

  uint8_t* data_ptr = (uint8_t*)img->data;
  int channels      = img->channels();
  cv::Scalar *color = new cv::Scalar();

  for(int i = 0; i < channels; i++) {
    (*color)[i] = data_ptr[(coords->y * img->cols * channels) + (coords->x * channels) + i];
  }

  return cv_color_to_rb_color(color);
}

VALUE
rb_img_get_cols(VALUE self) {
  cv::Mat *img = SG_GET_IMAGE(self);
  return INT2FIX(img->cols);
}

VALUE
rb_img_get_contours(VALUE self) {
  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat tmp(*img);

  std::vector<std::vector<cv::Point> > contours;
  cv::findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

  return cv_contours_to_rb_contours(contours);
}

cv::Mat *
_do_convert(VALUE self, VALUE color_space, bool inplace) {
  int code = FIX2INT(color_space);

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;
  if(inplace)
    dest = img;
  else
    dest = new cv::Mat();

  cvtColor(*img, *dest, code);
  return dest;
}

VALUE
rb_img_convert(VALUE self, VALUE color_space) {
  cv::Mat *img = _do_convert(self, color_space, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_convert_inplace(VALUE self, VALUE color_space) {
  cv::Mat *img = _do_convert(self, color_space, true);
  return self;
}

cv::Mat *
_do_crop(VALUE self, VALUE rect) {
  if(CLASS_OF(rect) != rb_cRect) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Rect)",
        rb_obj_classname(rect));
  }

  cv::Mat *img          = SG_GET_IMAGE(self);
  cv::Rect *boundaries  = SG_GET_RECT(rect);
  cv::Mat *dest         = new cv::Mat();

  (*img)(*boundaries).copyTo(*dest);
  return dest;
}

VALUE
rb_img_crop(VALUE self, VALUE rect) {
  cv::Mat *img = _do_crop(self, rect);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_crop_inplace(VALUE self, VALUE rect) {
  // This difers slightly from the other methods for memory reasons.
  // Whenever you crop an image, OpenCV doesn't automatically reallocate
  // memory for a smaller size, so we just copy the pixels into a new
  // instance and get rid of the old one.
  cv::Mat *old_img = SG_GET_IMAGE(self);
  cv::Mat *img     = _do_crop(self, rect);
  rb_img_free(old_img);

  Data_Set_Struct(self, img);
  return self;
}

VALUE
rb_img_mean(int argc, VALUE *argv, VALUE self) {
  VALUE mask;
  rb_scan_args(argc, argv, "01", &mask);

  if(RTEST(mask) && CLASS_OF(mask) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(mask));
  }

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Scalar mean;

  if(RTEST(mask)) {
    cv::Mat *_mask = SG_GET_IMAGE(mask);
    mean = cv::mean(*img, *_mask);
  } else {
    mean = cv::mean(*img);
  }

  cv::Scalar *_mean = new cv::Scalar(mean);
  return cv_color_to_rb_color(_mean);
}

VALUE
rb_img_get_rows(VALUE self) {
  cv::Mat *img = SG_GET_IMAGE(self);
  return INT2FIX(img->rows);
}

VALUE
rb_img_get_size(VALUE self) {
  cv::Mat *img = SG_GET_IMAGE(self);
  return cv_img_to_rb_size(img);
}

cv::Mat *
_do_resize(VALUE self, VALUE size, bool inplace) {
  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;

  cv::Size *_size = SG_GET_SIZE(size);

  if(inplace)
    dest = img;
  else
    dest = new cv::Mat();

  cv::resize(*img, *dest, *_size);

  return dest;
}

VALUE
rb_img_resize(VALUE self, VALUE size) {
  cv::Mat *img = _do_resize(self, size, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_resize_inplace(VALUE self, VALUE size) {
  _do_resize(self, size, true);
  return self;
}


cv::Mat *
_do_threshold(int argc, VALUE *argv, VALUE self, bool inverse, bool inplace) {
  VALUE threshold, replacement, opts;
  rb_scan_args(argc, argv, "21", &threshold, &replacement, &opts);

  Check_Type(threshold, T_FLOAT);
  Check_Type(replacement, T_FLOAT);

  cv::Mat *img = SG_GET_IMAGE(self);
  cv::Mat *dest;

  int method = inverse ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY;

  if(inplace)
    dest = img;
  else
    dest = new cv::Mat();

  cv::threshold(*img, *dest, NUM2DBL(threshold), NUM2DBL(replacement), method);
  return dest;
}

VALUE
rb_img_threshold(int argc, VALUE *argv, VALUE self) {
  cv::Mat *img = _do_threshold(argc, argv, self, false, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_threshold_inplace(int argc, VALUE *argv, VALUE self) {
  _do_threshold(argc, argv, self, false, true);
  return self;
}

VALUE
rb_img_threshold_inv(int argc, VALUE *argv, VALUE self) {
  cv::Mat *img = _do_threshold(argc, argv, self, true, false);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
rb_img_threshold_inv_inplace(int argc, VALUE *argv, VALUE self) {
  _do_threshold(argc, argv, self, true, true);
  return self;
}

VALUE
rb_img_warp_perspective(VALUE self, VALUE corners, VALUE size) {
  std::vector<cv::Point> _corners = cv_contour_to_cv_points(SG_GET_CONTOUR(corners));
  cv::Size *_size = SG_GET_SIZE(size);
  cv::Mat *img    = SG_GET_IMAGE(self);
  cv::Mat *result = new cv::Mat(_size->height, _size->width, CV_8UC3);

  // Convert the corner points to `Point2f`, as `getPerspectiveTransform` only
  // works with that kind of array.
  std::vector<cv::Point2f> corner_pts;
  for(int i = 0; i < _corners.size(); i++) {
    cv::Point2f pt;
    pt.x = _corners[i].x;
    pt.y = _corners[i].y;
    corner_pts.push_back(pt);
  }

  std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(_size->width, 0));
  quad_pts.push_back(cv::Point2f(_size->width, _size->height));
  quad_pts.push_back(cv::Point2f(0, _size->height));

 	cv::Mat transmtx = cv::getPerspectiveTransform(corner_pts, quad_pts);
	cv::warpPerspective(*img, *result, transmtx, result->size());
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, result);
}

VALUE
rb_img_write(VALUE self, VALUE filename) {
  Check_Type(filename, T_STRING);

  cv::Mat *img = SG_GET_IMAGE(self);
  bool res = cv::imwrite(StringValueCStr(filename), *img);
  return (res) ? Qtrue : Qfalse;
}

VALUE
rb_img_zeros(int argc, VALUE *argv, VALUE klass) {
  VALUE size, type;
  rb_scan_args(argc, argv, "11", &size, &type);

  if(CLASS_OF(size) != rb_cSize) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Size)",
        rb_obj_classname(size));
  }

  cv::Size *_size = SG_GET_SIZE(size);

  int _type = RTEST(type) ? NUM2INT(type) : CV_8UC1;

  cv::Mat zeros = cv::Mat::zeros(*_size, _type);
  cv::Mat *img  = new cv::Mat(zeros);
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, img);
}

VALUE
cv_img_to_rb_img(cv::Mat *mat) {
  return Data_Wrap_Struct(rb_cImage, NULL, rb_img_free, mat);
}

