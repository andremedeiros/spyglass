#ifndef SPYGLASS_CASCADE_CLASSIFIER_H_
#define SPYGLASS_CASCADE_CLASSIFIER_H_

#include "spyglass.h"

namespace Spyglass {
  namespace CascadeClassifier {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(cv::CascadeClassifier *classifier);
    static VALUE rb_initialize(VALUE self, VALUE src);
    static VALUE rb_detect(int argc, VALUE *argv, VALUE self);
  }
}

#endif // SPYGLASS_CASCADE_CLASSIFIER_H_
