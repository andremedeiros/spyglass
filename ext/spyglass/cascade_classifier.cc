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
      rb_define_method(CascadeClassifierClass, "detect", RUBY_METHOD_FUNC(rb_detect), 1);
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

      SG_GET_CLASSIFIER(self, classifier);

      classifier->load(StringValueCStr(src));

      return self;
    }

    static VALUE rb_detect(VALUE self, VALUE image) {
      if(CLASS_OF(image) != Spyglass::Image::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)", rb_obj_classname(image));
      }

      SG_GET_CLASSIFIER(self, classifier);
      SG_GET_IMAGE(image, img);

      std::vector<cv::Rect> results;
      classifier->detectMultiScale(*img, results);

      VALUE result = rb_ary_new2(results.size());
      for(size_t i = 0; i < results.size(); i++) {
        rb_ary_store(result, (long)i, Spyglass::Rect::rect_from_cvrect(&results[i]));
      }

      return result;
    }
  }
}
