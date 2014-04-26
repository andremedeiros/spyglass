#include "background_subtractor.h"

extern VALUE rb_mSpyglass;
extern VALUE rb_cImage;

VALUE rb_mBackgroundSubtractor = Qnil;
VALUE rb_cBGSMOG = Qnil;
VALUE rb_cBGSMOG2 = Qnil;
VALUE rb_cBGSGMG = Qnil;
VALUE rb_cBGSPratiMediod = Qnil;

void
rb_bgs_init() {
  // Module definition
  rb_mBackgroundSubtractor = rb_define_module_under(rb_mSpyglass, "BackgroundSubtractor");

  // Class definition: MOG
  rb_cBGSMOG = rb_define_class_under(rb_mBackgroundSubtractor, "MOG", rb_cObject);
  rb_define_alloc_func(rb_cBGSMOG, rb_bgs_mog_alloc);
  rb_define_method(rb_cBGSMOG, "initialize", RUBY_METHOD_FUNC(rb_bgs_initialize), -1);
  rb_define_method(rb_cBGSMOG, "subtract", RUBY_METHOD_FUNC(rb_bgs_subtract), -1);

  // Class definition: MOG2
  rb_cBGSMOG2 = rb_define_class_under(rb_mBackgroundSubtractor, "MOG2", rb_cObject);
  rb_define_alloc_func(rb_cBGSMOG2, rb_bgs_mog2_alloc);
  rb_define_method(rb_cBGSMOG2, "initialize", RUBY_METHOD_FUNC(rb_bgs_initialize), -1);
  rb_define_method(rb_cBGSMOG2, "subtract", RUBY_METHOD_FUNC(rb_bgs_subtract), -1);

  // Class definition: GMG
  rb_cBGSGMG = rb_define_class_under(rb_mBackgroundSubtractor, "GMG", rb_cObject);
  rb_define_alloc_func(rb_cBGSGMG, rb_bgs_gmg_alloc);
  rb_define_method(rb_cBGSGMG, "initialize", RUBY_METHOD_FUNC(rb_bgs_initialize), -1);
  rb_define_method(rb_cBGSGMG, "subtract", RUBY_METHOD_FUNC(rb_bgs_subtract), -1);

  // Class definition: PratiMediod
  rb_cBGSPratiMediod = rb_define_class_under(rb_mBackgroundSubtractor, "PratiMediod", rb_cObject);
  rb_define_alloc_func(rb_cBGSPratiMediod, rb_bgs_prati_mediod_alloc);
  rb_define_method(rb_cBGSPratiMediod, "initialize", RUBY_METHOD_FUNC(rb_bgs_initialize), -1);
  rb_define_method(rb_cBGSPratiMediod, "subtract", RUBY_METHOD_FUNC(rb_bgs_subtract), -1);
}

void
rb_bgs_free(cv::BackgroundSubtractor *bg) {
  delete bg;
}

VALUE
rb_bgs_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE r_opts;
  rb_scan_args(argc, argv, "01", &r_opts);

  if(!RTEST(r_opts))
    return self;

  cv::BackgroundSubtractor *bg = SG_GET_BG_SUBTRACTOR(self);

  if(CLASS_OF(self) == rb_cBGSMOG) {
    SG_OPTION(r_opts, int,    history,          200,  NUM2INT);
    SG_OPTION(r_opts, int,    mixtures,         5,    NUM2INT);
    SG_OPTION(r_opts, double, background_ratio, 0.7,  NUM2DBL);

    bg->set("history", history);
    bg->set("nmixtures", mixtures);
    bg->set("backgroundRatio", background_ratio);

  } else if(CLASS_OF(self) == rb_cBGSMOG2) {
    SG_OPTION(r_opts, int,    history,          500,  NUM2INT);
    SG_OPTION(r_opts, double, threshold,        8.0,  NUM2DBL);
    SG_OPTION(r_opts, bool,   shadow_detection, true, RTEST);

    bg->set("history", history);
    bg->set("varThreshold", threshold);
    bg->set("detectShadows", shadow_detection);

  } else if(CLASS_OF(self) == rb_cBGSPratiMediod) {
    SG_OPTION(r_opts, int, history,       16, NUM2INT);
    SG_OPTION(r_opts, int, threshold,     30, NUM2INT);
    SG_OPTION(r_opts, int, sampling_rate, 5,  NUM2INT);

    bg->set("historySize", history);
    bg->set("threshold", threshold);
    bg->set("samplingRate", sampling_rate);
  }

  return self;
}

VALUE
rb_bgs_subtract(int argc, VALUE *argv, VALUE self) {
  VALUE r_image, r_learn_rate;
  double rate;

  rb_scan_args(argc, argv, "11", &r_image, &r_learn_rate);

  if(CLASS_OF(r_image) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(r_image));
  }

  if(RTEST(r_learn_rate)) {
    if(TYPE(r_learn_rate) != T_FLOAT && TYPE(r_learn_rate) != T_FIXNUM) {
      rb_raise(rb_eTypeError, "wrong argument type %s (expected Float or Fixnum)",
          rb_obj_classname(r_learn_rate));
    } else {
      rate = NUM2DBL(r_learn_rate);
    }
  } else {
    rate = -1.0;
  }

  cv::BackgroundSubtractor *bg  = SG_GET_BG_SUBTRACTOR(self);
  cv::Mat *img                  = SG_GET_IMAGE(r_image);

  cv::Mat *delta = new cv::Mat();
  (*bg)(*img, *delta, rate);

  return cv_img_to_rb_img(delta);
}

VALUE
rb_bgs_mog_alloc(VALUE self) {
  cv::BackgroundSubtractor *bg = new cv::BackgroundSubtractorMOG();
  return Data_Wrap_Struct(rb_cBGSMOG, NULL, rb_bgs_free, bg);
}

VALUE
rb_bgs_mog2_alloc(VALUE self) {
  cv::BackgroundSubtractor *bg = new cv::BackgroundSubtractorMOG2();
  return Data_Wrap_Struct(rb_cBGSMOG2, NULL, rb_bgs_free, bg);
}

VALUE
rb_bgs_gmg_alloc(VALUE self) {
  cv::BackgroundSubtractor *bg = new cv::BackgroundSubtractorGMG();
  return Data_Wrap_Struct(rb_cBGSGMG, NULL, rb_bgs_free, bg);
}

VALUE
rb_bgs_prati_mediod_alloc(VALUE self) {
  cv::BackgroundSubtractor *bg = new DPPratiMediodBGS();
  return Data_Wrap_Struct(rb_cBGSPratiMediod, NULL, rb_bgs_free, bg);
}
