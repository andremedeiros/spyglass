#ifndef ROPENCV_VIDEO_CAPTURE_H_
#define ROPENCV_VIDEO_CAPTURE_H_

#include "opencv.h"

namespace OpenCV {
  namespace VideoCapture {
    void define_ruby_class();
    VALUE get_ruby_class();
    VALUE rb_opencv_video_capture_alloc(VALUE self);
    void rb_opencv_video_capture_free(cv::VideoCapture *cap);

    VALUE rb_opencv_video_capture_initialize(VALUE self, VALUE src);
    VALUE rb_opencv_video_capture_capture(VALUE self, VALUE dest);
    VALUE rb_opencv_video_capture_get_open(VALUE self);
    VALUE rb_opencv_video_capture_stop(VALUE self);
  }
}

#endif
