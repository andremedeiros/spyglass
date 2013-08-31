#include "cascade_classifier.h"

static VALUE CascadeClassifierClass;

namespace Spyglass {
  namespace CascadeClassifier {
    void define_ruby_class() {
      // Class definition
      CascadeClassifierClass = rb_define_class_under(Spyglass::get_ruby_module(), "CascadeClassifier", rb_cObject);
      rb_define_alloc_func(CascadeClassifierClass, rb_alloc);
      rb_define_method(CascadeClassifierClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 1);

      // Instance methods
      rb_define_method(CascadeClassifierClass, "detect", RUBY_METHOD_FUNC(rb_detect), -1);
    }

    VALUE get_ruby_class() {
      return CascadeClassifierClass;
    }

    static VALUE rb_alloc(VALUE self) {
      cv::CascadeClassifier *classifier = new cv::CascadeClassifier();
      return Data_Wrap_Struct(CascadeClassifierClass, NULL, rb_free, classifier);
    }

    static void rb_free(cv::CascadeClassifier *classifier) {
      classifier->~CascadeClassifier();
      delete classifier;
    }

    static VALUE rb_initialize(VALUE self, VALUE src) {
      Check_Type(src, T_STRING);

      cv::CascadeClassifier *classifier = SG_GET_CLASSIFIER(self);

      classifier->load(StringValueCStr(src));

      return self;
    }

    static VALUE rb_detect(int argc, VALUE *argv, VALUE self) {
      VALUE image, opts;
      rb_scan_args(argc, argv, "11", &image, &opts);

      if(CLASS_OF(image) != Spyglass::Image::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)", rb_obj_classname(image));
      }

      cv::CascadeClassifier *classifier = SG_GET_CLASSIFIER(self);
      cv::Mat *img                      = SG_GET_IMAGE(image);

      SG_OPTION(opts, double,     scale_factor,   1.1,            NUM2DBL);
      SG_OPTION(opts, int,        min_neighbors,  3,              NUM2INT);
      SG_OPTION(opts, cv::Size *, min_size,       new cv::Size(), SG_GET_SIZE);
      SG_OPTION(opts, cv::Size *, max_size,       new cv::Size(), SG_GET_SIZE);

      std::vector<cv::Rect> results;
      classifier->detectMultiScale(*img, results, scale_factor, min_neighbors, 0, *min_size, *max_size);

      VALUE result = rb_ary_new2(results.size());
      for(size_t i = 0; i < results.size(); i++) {
        rb_ary_store(result, (long)i, Spyglass::Rect::rect_from_cvrect(&results[i]));
      }

      return result;
    }
  }
}
