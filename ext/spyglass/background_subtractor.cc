#include "background_subtractor.h"

// static VALUE BackgroundSubtractorClass;
static VALUE BackgroundSubtractorModule;
static VALUE BGSMOGClass;
static VALUE BGSMOG2Class;
static VALUE BGSGMGClass;
static VALUE BGSPratiMediodClass;

namespace Spyglass {
  namespace BackgroundSubtractor {
    void define_ruby_types() {
      // Module definition
      BackgroundSubtractorModule = rb_define_module_under(Spyglass::get_ruby_module(), "BackgroundSubtractor");

      // Class definition: MOG
      BGSMOGClass = rb_define_class_under(BackgroundSubtractorModule, "MOG", rb_cObject);
      rb_define_alloc_func(BGSMOGClass, rb_mog_alloc);
      rb_define_method(BGSMOGClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
      rb_define_method(BGSMOGClass, "subtract", RUBY_METHOD_FUNC(rb_subtract), -1);

      // Class definition: MOG2
      BGSMOG2Class = rb_define_class_under(BackgroundSubtractorModule, "MOG2", rb_cObject);
      rb_define_alloc_func(BGSMOG2Class, rb_mog2_alloc);
      rb_define_method(BGSMOG2Class, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
      rb_define_method(BGSMOG2Class, "subtract", RUBY_METHOD_FUNC(rb_subtract), -1);

      // Class definition: GMG
      BGSGMGClass = rb_define_class_under(BackgroundSubtractorModule, "GMG", rb_cObject);
      rb_define_alloc_func(BGSGMGClass, rb_gmg_alloc);
      rb_define_method(BGSGMGClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
      rb_define_method(BGSGMGClass, "subtract", RUBY_METHOD_FUNC(rb_subtract), -1);

      // Class definition: PratiMediod
      BGSPratiMediodClass = rb_define_class_under(BackgroundSubtractorModule, "PratiMediod", rb_cObject);
      rb_define_alloc_func(BGSPratiMediodClass, rb_prati_mediod_alloc);
      rb_define_method(BGSPratiMediodClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
      rb_define_method(BGSPratiMediodClass, "subtract", RUBY_METHOD_FUNC(rb_subtract), -1);
    }

    VALUE get_ruby_module() {
      return BackgroundSubtractorModule;
    }

    static VALUE rb_mog_alloc(VALUE self) {
      cv::BackgroundSubtractor *bg = new cv::BackgroundSubtractorMOG();
      return Data_Wrap_Struct(BGSMOGClass, NULL, rb_free, bg);
    }

    static VALUE rb_mog2_alloc(VALUE self) {
      cv::BackgroundSubtractor *bg = new cv::BackgroundSubtractorMOG2();
      return Data_Wrap_Struct(BGSMOG2Class, NULL, rb_free, bg);
    }

    static VALUE rb_gmg_alloc(VALUE self) {
      cv::BackgroundSubtractor *bg = new cv::BackgroundSubtractorGMG();
      return Data_Wrap_Struct(BGSGMGClass, NULL, rb_free, bg);
    }

    static VALUE rb_prati_mediod_alloc(VALUE self) {
      cv::BackgroundSubtractor *bg = new DPPratiMediodBGS();
      return Data_Wrap_Struct(BGSPratiMediodClass, NULL, rb_free, bg);
    }

    static void rb_free(cv::BackgroundSubtractor *bg) {
      delete bg;
    }

    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self) {
      VALUE opts;
      rb_scan_args(argc, argv, "01", &opts);

      if(!RTEST(opts))
        return self;

      cv::BackgroundSubtractor *bg = SG_GET_BG_SUBTRACTOR(self);

      if(CLASS_OF(self) == BGSMOGClass) {
        SG_OPTION(opts, int,    history,          200,  NUM2INT);
        SG_OPTION(opts, int,    mixtures,         5,    NUM2INT);
        SG_OPTION(opts, double, background_ratio, 0.7,  NUM2DBL);

        bg->set("history", history);
        bg->set("nmixtures", mixtures);
        bg->set("backgroundRatio", background_ratio);
      } else if(CLASS_OF(self) == BGSMOG2Class) {
        SG_OPTION(opts, int,    history,          500,  NUM2INT);
        SG_OPTION(opts, double, threshold,        8.0,  NUM2DBL);
        SG_OPTION(opts, bool,   shadow_detection, true, RTEST);

        bg->set("history", history);
        bg->set("varThreshold", threshold);
        bg->set("detectShadows", shadow_detection);
      } else if(CLASS_OF(self) == BGSPratiMediodClass) {
        SG_OPTION(opts, int, history,   16, NUM2INT);
        SG_OPTION(opts, int, threshold, 30, NUM2INT);

        bg->set("historySize", history);
        bg->set("threshold", threshold);
      }

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

      cv::BackgroundSubtractor *bg  = SG_GET_BG_SUBTRACTOR(self);
      cv::Mat *img                  = SG_GET_IMAGE(image);

      cv::Mat *delta = new cv::Mat();
      (*bg)(*img, *delta, rate);

      return Image::from_cvmat(delta);
    }
  }
}
