#include "opencv.h"

static VALUE OpenCVModule;

namespace OpenCV {

  VALUE get_ruby_module() {
    return OpenCVModule;
  }

  void define_ruby_module() {
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

    OpenCV::GUI::define_ruby_module();
    OpenCV::GUI::Window::define_ruby_class();
  }
}
