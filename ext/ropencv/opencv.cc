#include "opencv.h"

static VALUE OpenCVModule;

namespace OpenCV {

  VALUE rb_opencv_module() {
    return OpenCVModule;
  }

  void define_ruby_module() {
    if(OpenCVModule) return;
    OpenCVModule = rb_define_module("OpenCV");
  }

}

extern "C" {
  void Init_ropencv() {
    OpenCV::define_ruby_module();
    OpenCV::Image::define_ruby_class();
    OpenCV::Point::define_ruby_class();
    OpenCV::Rect::define_ruby_class();
    OpenCV::Size::define_ruby_class();
  }
}
