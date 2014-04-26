#include "cascade_classifier.h"

extern VALUE rb_mSpyglass;
extern VALUE rb_cImage;

VALUE rb_cCascadeClassifier = Qnil;

void
rb_cc_init() {
  // Class definition
  rb_cCascadeClassifier = rb_define_class_under(rb_mSpyglass, "CascadeClassifier", rb_cObject);
  rb_define_alloc_func(rb_cCascadeClassifier, rb_cc_alloc);
  rb_define_method(rb_cCascadeClassifier, "initialize", RUBY_METHOD_FUNC(rb_cc_initialize), 1);

  // Instance methods
  rb_define_method(rb_cCascadeClassifier, "detect", RUBY_METHOD_FUNC(rb_cc_detect), -1);
}

VALUE
rb_cc_alloc(VALUE self) {
  cv::CascadeClassifier *classifier = new cv::CascadeClassifier();
  return Data_Wrap_Struct(rb_cCascadeClassifier, NULL, rb_cc_free, classifier);
}

void
rb_cc_free(cv::CascadeClassifier *classifier) {
  classifier->~CascadeClassifier();
  delete classifier;
}

VALUE
rb_cc_initialize(VALUE self, VALUE r_src) {
  Check_Type(r_src, T_STRING);

  cv::CascadeClassifier *classifier = SG_GET_CLASSIFIER(self);

  classifier->load(StringValueCStr(r_src));

  return self;
}

VALUE
rb_cc_detect(int argc, VALUE *argv, VALUE self) {
  VALUE r_image, opts;
  rb_scan_args(argc, argv, "11", &r_image, &opts);

  if(CLASS_OF(r_image) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(r_image));
  }

  cv::CascadeClassifier *classifier = SG_GET_CLASSIFIER(self);
  cv::Mat *img                      = SG_GET_IMAGE(r_image);

  SG_OPTION(opts, double,     scale_factor,   1.1,            NUM2DBL);
  SG_OPTION(opts, int,        min_neighbors,  3,              NUM2INT);
  SG_OPTION(opts, cv::Size *, min_size,       new cv::Size(), SG_GET_SIZE);
  SG_OPTION(opts, cv::Size *, max_size,       new cv::Size(), SG_GET_SIZE);

  std::vector<cv::Rect> results;
  classifier->detectMultiScale(*img, results, scale_factor, min_neighbors, 0, *min_size, *max_size);

  VALUE result = rb_ary_new2(results.size());
  for(size_t i = 0; i < results.size(); i++) {
    cv::Rect *rect = new cv::Rect(results[i]);
    rb_ary_store(result, (long)i, cv_rect_to_rb_rect(rect));
  }

  return result;
}
