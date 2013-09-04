#include "background_subtractor.h"

static VALUE BackgroundSubtractorClass;

namespace Spyglass {
  namespace BackgroundSubtractor {
    void define_ruby_class() {
      // Class definition
      BackgroundSubtractorClass = rb_define_class_under(Spyglass::get_ruby_module(), "BackgroundSubtractor", rb_cObject);
      rb_define_alloc_func(BackgroundSubtractorClass, rb_alloc);
      rb_define_method(BackgroundSubtractorClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);

      // Instance methods
      rb_define_method(BackgroundSubtractorClass, "subtract", RUBY_METHOD_FUNC(rb_subtract), -1);
    }

    VALUE get_ruby_class() {
      return BackgroundSubtractorClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::BackgroundSubtractorMOG2 *bg = new cv::BackgroundSubtractorMOG2();
      return Data_Wrap_Struct(BackgroundSubtractorClass, NULL, rb_free, bg);
    }

    static void rb_free(cv::BackgroundSubtractorMOG2 *bg) {
      bg->~BackgroundSubtractorMOG2();
      delete bg;
    }

    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self) {
      VALUE opts;
      rb_scan_args(argc, argv, "01", &opts);

      if(!RTEST(opts))
        return self;

      cv::BackgroundSubtractorMOG2 *bg = SG_GET_BG_SUBTRACTOR(self);

      SG_OPTION(opts, int,    history,          500,  NUM2INT);
      SG_OPTION(opts, double, threshold,        8.0,  NUM2DBL);
      SG_OPTION(opts, bool,   shadow_detection, true, RTEST);

      bg->set("history", history);
      bg->set("varThreshold", threshold);
      bg->set("detectShadows", shadow_detection);

      return self;
    }

    static VALUE rb_subtract(int argc, VALUE *argv, VALUE self) {
      VALUE image, learn_rate;
      double rate;

      rb_scan_args(argc, argv, "11", &image, &learn_rate);

      if(CLASS_OF(image) != Image::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
            rb_obj_classname(image));
      }


      if(RTEST(learn_rate) && TYPE(learn_rate) != T_FLOAT && TYPE(learn_rate) != T_FIXNUM)
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Float or Fixnum)",
              rb_obj_classname(learn_rate));

      rate = RTEST(learn_rate) ? NUM2DBL(learn_rate) : -1.0;

      cv::BackgroundSubtractorMOG2 *bg  = SG_GET_BG_SUBTRACTOR(self);
      cv::Mat *img                      = SG_GET_IMAGE(image);

      cv::Mat *delta = new cv::Mat();
      (*bg)(*img, *delta, rate);

      return Image::from_cvmat(delta);
    }
  }
}
