#include "cascade_classifier.h"

static VALUE CascadeClassifierClass;

namespace OpenCV {
  namespace CascadeClassifier {
    void define_ruby_class() {
      // Class definition
      CascadeClassifierClass = rb_define_class_under(OpenCV::get_ruby_module(), "CascadeClassifier", rb_cObject);
      rb_define_alloc_func(CascadeClassifierClass, rb_opencv_cascade_classifier_alloc);
      rb_define_method(CascadeClassifierClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_cascade_classifier_initialize), 1);

      // Instance methods
      rb_define_method(CascadeClassifierClass, "detect", RUBY_METHOD_FUNC(rb_opencv_cascade_classifier_detect), 1);
    }

    VALUE get_ruby_class() {
      return CascadeClassifierClass;
    }

    VALUE rb_opencv_cascade_classifier_alloc(VALUE self) {
      cv::CascadeClassifier *classifier = new cv::CascadeClassifier();
      return Data_Wrap_Struct(CascadeClassifierClass, NULL, rb_opencv_cascade_classifier_free, classifier);
    }

    void rb_opencv_cascade_classifier_free(cv::CascadeClassifier *classifier) {
      classifier->~CascadeClassifier();
      delete classifier;
    }

    VALUE rb_opencv_cascade_classifier_initialize(VALUE self, VALUE src) {
      Check_Type(src, T_STRING);

      cv::CascadeClassifier *classifier;
      Data_Get_Struct(self, cv::CascadeClassifier, classifier);

      classifier->load(StringValueCStr(src));

      return self;
    }

    VALUE rb_opencv_cascade_classifier_detect(VALUE self, VALUE image) {
      if(CLASS_OF(image) != OpenCV::Image::get_ruby_class()) {
        rb_raise(rb_eTypeError, "wrong argument type %s (expected OpenCV::Image)", rb_obj_classname(image));
      }

      cv::CascadeClassifier *classifier;
      Data_Get_Struct(self, cv::CascadeClassifier, classifier);

      cv::Mat *img;
      Data_Get_Struct(image, cv::Mat, img);

      std::vector<cv::Rect> results;
      classifier->detectMultiScale(*img, results);

      VALUE result = rb_ary_new2(results.size());
      for(size_t i = 0; i < results.size(); i++) {
        rb_ary_store(result, (long)i, OpenCV::Rect::rb_opencv_rect_from_cvrect(&results[i]));
      }

      return result;
    }
  }
}
