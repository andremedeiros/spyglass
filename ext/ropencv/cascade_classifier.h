#ifndef ROPENCV_CASCADE_CLASSIFIER_H_
#define ROPENCV_CASCADE_CLASSIFIER_H_

#include "opencv.h"

#include <opencv2/opencv.hpp>

namespace OpenCV {
  namespace CascadeClassifier {
    void define_ruby_class();
    VALUE get_ruby_class();
    VALUE rb_opencv_cascade_classifier_alloc(VALUE self);
    void rb_opencv_cascade_classifier_free(cv::CascadeClassifier *classifier);

    VALUE rb_opencv_cascade_classifier_initialize(VALUE self, VALUE src);
    VALUE rb_opencv_cascade_classifier_detect(VALUE self, VALUE image);
  }
}

#endif
