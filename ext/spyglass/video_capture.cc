#include "video_capture.h"

extern VALUE rb_mSpyglass;
extern VALUE rb_cImage;

VALUE rb_cVideoCapture = Qnil;

void rb_cap_init() {
  rb_cVideoCapture = rb_define_class_under(rb_mSpyglass, "VideoCapture", rb_cObject);
  rb_define_alloc_func(rb_cVideoCapture, rb_cap_alloc);
  rb_define_method(rb_cVideoCapture, "initialize", RUBY_METHOD_FUNC(rb_cap_initialize), -1);

  // Instance methods
  rb_define_method(rb_cVideoCapture, ">>", RUBY_METHOD_FUNC(rb_cap_capture), 1);
  rb_define_method(rb_cVideoCapture, "open?", RUBY_METHOD_FUNC(rb_cap_is_open), 0);
  rb_define_method(rb_cVideoCapture, "stop", RUBY_METHOD_FUNC(rb_cap_stop), 0);
}

VALUE
rb_cap_alloc(VALUE self) {
  cv::VideoCapture *cap = new cv::VideoCapture();
  return Data_Wrap_Struct(rb_cVideoCapture, NULL, rb_cap_free, cap);
}

void
rb_cap_free(cv::VideoCapture *cap) {
  if(cap->isOpened())
    cap->release();

  cap->~VideoCapture();
  delete cap;
}

VALUE
rb_cap_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE src, opts;
  rb_scan_args(argc, argv, "11", &src, &opts);

  cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);

  SG_OPTION(opts, double, width,   0, NUM2DBL);
  SG_OPTION(opts, double, height,  0, NUM2DBL);
  SG_OPTION(opts, double, fps,     0, NUM2DBL);

  int type = TYPE(src);

  switch(type) {
    case T_FIXNUM:
      cap->open(FIX2INT(src));
      break;
    case T_STRING:
      cap->open(StringValueCStr(src));
      break;
    default:
      rb_raise(rb_eTypeError, "wrong argument type %s (expected Fixnum or String)",
          rb_obj_classname(src));
  }

  if(width > 0)
    cap->set(CV_CAP_PROP_FRAME_WIDTH, width);

  if(height > 0)
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, height);

  if(fps > 0)
    cap->set(CV_CAP_PROP_FPS, fps);

  return self;
}

VALUE
rb_cap_capture(VALUE self, VALUE r_dest) {
  if(CLASS_OF(r_dest) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(r_dest));
  }

  cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);
  cv::Mat *img          = SG_GET_IMAGE(r_dest);

  (*cap) >> (*img);

  return r_dest;
}

VALUE
rb_cap_is_open(VALUE self) {
  cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);
  return (cap->isOpened()) ? Qtrue : Qfalse;
}

VALUE
rb_cap_stop(VALUE self) {
  cv::VideoCapture *cap = SG_GET_VIDEO_CAPTURE(self);
  cap->release();
  return self;
}
