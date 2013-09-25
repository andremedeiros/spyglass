#include "image.h"

static VALUE ImageClass;

namespace Spyglass {
  namespace Image {
    void define_ruby_class() {
      // Class definition
      ImageClass = rb_define_class_under(Spyglass::get_ruby_module(), "Image", rb_cObject);
      rb_define_alloc_func(ImageClass, rb_alloc);
      rb_define_method(ImageClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);

      // Class methods
      rb_define_singleton_method(ImageClass, "load", RUBY_METHOD_FUNC(rb_load), -1);
      rb_define_singleton_method(ImageClass, "zeros", RUBY_METHOD_FUNC(rb_zeros), -1);

      // Instance methods
      rb_define_method(ImageClass, "canny", RUBY_METHOD_FUNC(rb_canny), 2);
      rb_define_method(ImageClass, "canny!", RUBY_METHOD_FUNC(rb_canny_inplace), 2);
      rb_define_method(ImageClass, "cols", RUBY_METHOD_FUNC(rb_get_cols), 0);
      rb_define_method(ImageClass, "contours", RUBY_METHOD_FUNC(rb_get_contours), 0);
      rb_define_method(ImageClass, "convert", RUBY_METHOD_FUNC(rb_convert), 1);
      rb_define_method(ImageClass, "convert!", RUBY_METHOD_FUNC(rb_convert_inplace), 1);
      rb_define_method(ImageClass, "copy!", RUBY_METHOD_FUNC(rb_copy_inplace), -1);
      rb_define_method(ImageClass, "crop", RUBY_METHOD_FUNC(rb_crop), 1);
      rb_define_method(ImageClass, "crop!", RUBY_METHOD_FUNC(rb_crop_inplace), 1);
      rb_define_method(ImageClass, "dilate", RUBY_METHOD_FUNC(rb_dilate), -1);
      rb_define_method(ImageClass, "dilate!", RUBY_METHOD_FUNC(rb_dilate_inplace), -1);
      rb_define_method(ImageClass, "draw_contours", RUBY_METHOD_FUNC(rb_draw_contours), 2);
      rb_define_method(ImageClass, "draw_rectangle", RUBY_METHOD_FUNC(rb_draw_rectangle), 2);
      rb_define_method(ImageClass, "erode", RUBY_METHOD_FUNC(rb_erode), -1);
      rb_define_method(ImageClass, "erode!", RUBY_METHOD_FUNC(rb_erode_inplace), -1);
      rb_define_method(ImageClass, "fill", RUBY_METHOD_FUNC(rb_fill), -1);
      rb_define_method(ImageClass, "fill!", RUBY_METHOD_FUNC(rb_fill_inplace), -1);
      rb_define_method(ImageClass, "mean", RUBY_METHOD_FUNC(rb_mean), -1);
      rb_define_method(ImageClass, "resize", RUBY_METHOD_FUNC(rb_resize), 1);
      rb_define_method(ImageClass, "resize!", RUBY_METHOD_FUNC(rb_resize_inplace), 1);
      rb_define_method(ImageClass, "rows", RUBY_METHOD_FUNC(rb_get_rows), 0);
      rb_define_method(ImageClass, "size", RUBY_METHOD_FUNC(rb_get_size), 0);
      rb_define_method(ImageClass, "threshold", RUBY_METHOD_FUNC(rb_threshold), -1);
      rb_define_method(ImageClass, "threshold!", RUBY_METHOD_FUNC(rb_threshold_inplace), -1);
      rb_define_method(ImageClass, "threshold_inv", RUBY_METHOD_FUNC(rb_threshold_inv), -1);
      rb_define_method(ImageClass, "threshold_inv!", RUBY_METHOD_FUNC(rb_threshold_inv_inplace), -1);
      rb_define_method(ImageClass, "warp_perspective", RUBY_METHOD_FUNC(rb_warp_perspective), 2);
      rb_define_method(ImageClass, "write", RUBY_METHOD_FUNC(rb_write), 1);

      // Constants
      rb_define_const(ImageClass, "TYPE_8UC1", INT2NUM(CV_8UC1));
      rb_define_const(ImageClass, "TYPE_8UC3", INT2NUM(CV_8UC3));
    }

    VALUE get_ruby_class() {
      return ImageClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::Mat *mat = new cv::Mat();
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, mat);
    }

    static void rb_free(cv::Mat *img) {
      img->~Mat();
      delete img;
    }

    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self) {
      if(argc == 0)
        return self;

      VALUE size, type;
      rb_scan_args(argc, argv, "02", &size, &type);

      if(RTEST(size) && CLASS_OF(size) != Size::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Size)",
            rb_obj_classname(size));
      }

      if(RTEST(type))
        Check_Type(type, T_FIXNUM);

      cv::Mat *img = SG_GET_IMAGE(self);
      rb_free(img);

      cv::Size *_size = RTEST(size) ? SG_GET_SIZE(size) : new cv::Size();
      int _type       = RTEST(type) ? NUM2INT(type) : CV_8UC3;

      img = new cv::Mat(*_size, _type);
      Data_Set_Struct(self, img);

      return self;
    }

    cv::Mat *_do_canny(VALUE self, VALUE threshold1, VALUE threshold2, bool inplace) {
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

    static VALUE rb_canny(VALUE self, VALUE threshold1, VALUE threshold2) {
      cv::Mat *img = _do_canny(self, threshold1, threshold2, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_canny_inplace(VALUE self, VALUE threshold1, VALUE threshold2) {
      _do_canny(self, threshold1, threshold2, true);
      return self;
    }

    static VALUE rb_copy_inplace(int argc, VALUE *argv, VALUE self) {
      VALUE src, mask;
      rb_scan_args(argc, argv, "11", &src, &mask);

      if(CLASS_OF(src) != ImageClass) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
            rb_obj_classname(src));
      }

      if(RTEST(mask) && CLASS_OF(mask) != ImageClass) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Rect)",
            rb_obj_classname(mask));
      }

      cv::Mat *dest = SG_GET_IMAGE(self);

      rb_free(dest);
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

    cv::Mat *_do_dilate(int argc, VALUE *argv, VALUE self, bool inplace) {
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

    static VALUE rb_dilate(int argc, VALUE *argv, VALUE self) {
      cv::Mat *img = _do_dilate(argc, argv, self, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_dilate_inplace(int argc, VALUE *argv, VALUE self) {
      _do_dilate(argc, argv, self, true);
      return self;
    }

    static VALUE rb_draw_contours(VALUE self, VALUE contours, VALUE color) {
      if(TYPE(contours) != T_ARRAY && CLASS_OF(contours) != Contour::get_ruby_class())
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Array or Spyglass::Contour)",
            rb_obj_classname(contours));

      if(CLASS_OF(color) != Color::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Color)",
            rb_obj_classname(color));
      }

      cv::Mat *img        = SG_GET_IMAGE(self);
      cv::Scalar *_color  = SG_GET_COLOR(color);
      std::vector<std::vector<cv::Point> > ctrs;

      if(TYPE(contours) == T_ARRAY) {
        for(int idx = 0; idx < RARRAY_LEN(contours); idx++) {
          std::vector<cv::Point *> *contour = SG_GET_CONTOUR(rb_ary_entry(contours, idx));
          ctrs.push_back(Contour::to_value_vector(contour));
        }
      } else {
        std::vector<cv::Point *> *contour = SG_GET_CONTOUR(contours);
        ctrs.push_back(Contour::to_value_vector(contour));
      }

      for(int idx = 0; idx < ctrs.size(); idx++)
        cv::drawContours(*img, ctrs, idx, *_color, CV_FILLED);

      return self;
    }

    static VALUE rb_draw_rectangle(VALUE self, VALUE rect, VALUE color) {
      if(CLASS_OF(rect) != Rect::get_ruby_class()) {
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

    cv::Mat *_do_erode(int argc, VALUE *argv, VALUE self, bool inplace) {
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

    static VALUE rb_erode(int argc, VALUE *argv, VALUE self) {
      cv::Mat *img = _do_erode(argc, argv, self, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_erode_inplace(int argc, VALUE *argv, VALUE self) {
      _do_erode(argc, argv, self, true);
      return self;
    }

    cv::Mat *_do_fill(int argc, VALUE *argv, VALUE self, bool inplace) {
      VALUE color, mask;
      rb_scan_args(argc, argv, "11", &color, &mask);

      if(CLASS_OF(color) != Color::get_ruby_class())
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Color)",
            rb_obj_classname(color));

      if(RTEST(mask) && CLASS_OF(mask) != ImageClass)
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

    static VALUE rb_fill(int argc, VALUE *argv, VALUE self) {
      cv::Mat *img = _do_fill(argc, argv, self, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_fill_inplace(int argc, VALUE *argv, VALUE self) {
      _do_fill(argc, argv, self, true);
      return self;
    }

    static VALUE rb_load(int argc, VALUE *argv, VALUE klass) {
      VALUE filename;
      rb_scan_args(argc, argv, "10", &filename);
      Check_Type(filename, T_STRING);

      cv::Mat _img = cv::imread(StringValueCStr(filename));
      cv::Mat *img = new cv::Mat(_img);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_get_cols(VALUE self) {
      cv::Mat *img = SG_GET_IMAGE(self);
      return INT2FIX(img->cols);
    }

    static VALUE rb_get_contours(VALUE self) {
      cv::Mat *img = SG_GET_IMAGE(self);

      std::vector<std::vector<cv::Point> > contours;
      cv::findContours(*img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

      return Contour::from_contour_vector(contours);
    }

    cv::Mat *_do_convert(VALUE self, VALUE color_space, bool inplace) {
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

    static VALUE rb_convert(VALUE self, VALUE color_space) {
      cv::Mat *img = _do_convert(self, color_space, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_convert_inplace(VALUE self, VALUE color_space) {
      cv::Mat *img = _do_convert(self, color_space, true);
      return self;
    }

    cv::Mat *_do_crop(VALUE self, VALUE rect) {
      if(CLASS_OF(rect) != Rect::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Rect)",
            rb_obj_classname(rect));
      }

      cv::Mat *img          = SG_GET_IMAGE(self);
      cv::Rect *boundaries  = SG_GET_RECT(rect);
      cv::Mat *dest         = new cv::Mat();

      (*img)(*boundaries).copyTo(*dest);
      return dest;
    }

    static VALUE rb_crop(VALUE self, VALUE rect) {
      cv::Mat *img = _do_crop(self, rect);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_crop_inplace(VALUE self, VALUE rect) {
      // This difers slightly from the other methods for memory reasons.
      // Whenever you crop an image, OpenCV doesn't automatically reallocate
      // memory for a smaller size, so we just copy the pixels into a new
      // instance and get rid of the old one.
      cv::Mat *old_img = SG_GET_IMAGE(self);
      cv::Mat *img     = _do_crop(self, rect);
      rb_free(old_img);

      Data_Set_Struct(self, img);
      return self;
    }

    static VALUE rb_mean(int argc, VALUE *argv, VALUE self) {
      VALUE mask;
      rb_scan_args(argc, argv, "01", &mask);

      if(RTEST(mask) && CLASS_OF(mask) != Image::get_ruby_class()) {
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
      return Color::from_cvscalar(_mean);
    }

    static VALUE rb_get_rows(VALUE self) {
      cv::Mat *img = SG_GET_IMAGE(self);
      return INT2FIX(img->rows);
    }

    static VALUE rb_get_size(VALUE self) {
      cv::Mat *img = SG_GET_IMAGE(self);
      return Size::from_cvmat(img);
    }

    cv::Mat *_do_resize(VALUE self, VALUE size, bool inplace) {
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

    static VALUE rb_resize(VALUE self, VALUE size) {
      cv::Mat *img = _do_resize(self, size, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_resize_inplace(VALUE self, VALUE size) {
      _do_resize(self, size, true);
      return self;
    }


    cv::Mat *_do_threshold(int argc, VALUE *argv, VALUE self, bool inverse, bool inplace) {
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

    static VALUE rb_threshold(int argc, VALUE *argv, VALUE self) {
      cv::Mat *img = _do_threshold(argc, argv, self, false, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_threshold_inplace(int argc, VALUE *argv, VALUE self) {
      _do_threshold(argc, argv, self, false, true);
      return self;
    }

    static VALUE rb_threshold_inv(int argc, VALUE *argv, VALUE self) {
      cv::Mat *img = _do_threshold(argc, argv, self, true, false);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    static VALUE rb_threshold_inv_inplace(int argc, VALUE *argv, VALUE self) {
      _do_threshold(argc, argv, self, true, true);
      return self;
    }

    static VALUE rb_warp_perspective(VALUE self, VALUE corners, VALUE size) {
      std::vector<cv::Point> _corners = Contour::to_value_vector(SG_GET_CONTOUR(corners));
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
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, result);
    }

    static VALUE rb_write(VALUE self, VALUE filename) {
      Check_Type(filename, T_STRING);

      cv::Mat *img = SG_GET_IMAGE(self);
      bool res = cv::imwrite(StringValueCStr(filename), *img);
      return (res) ? Qtrue : Qfalse;
    }

    static VALUE rb_zeros(int argc, VALUE *argv, VALUE klass) {
      VALUE size, type;
      rb_scan_args(argc, argv, "11", &size, &type);

      if(CLASS_OF(size) != Size::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Size)",
            rb_obj_classname(size));
      }

      cv::Size *_size = SG_GET_SIZE(size);

      int _type = RTEST(type) ? NUM2INT(type) : CV_8UC1;

      cv::Mat zeros = cv::Mat::zeros(*_size, _type);
      cv::Mat *img  = new cv::Mat(zeros);
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, img);
    }

    VALUE from_cvmat(cv::Mat *mat) {
      return Data_Wrap_Struct(ImageClass, NULL, rb_free, mat);
    }
  }
}
