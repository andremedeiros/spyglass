#include "spyglass.h"

VALUE SpyglassModule = Qnil;

namespace Spyglass {

  void define_ruby_module() {
    SpyglassModule = rb_define_module("Spyglass");
  }

}

extern "C" {
  void Init_spyglass() {
    Spyglass::define_ruby_module();
    Spyglass::ColorSpace::define_ruby_module();

    Spyglass::BackgroundSubtractor::define_ruby_types();
    Spyglass::CascadeClassifier::define_ruby_class();
    Spyglass::Color::define_ruby_class();
    Spyglass::Contour::define_ruby_class();
    Spyglass::Image::define_ruby_class();
    Spyglass::Point::define_ruby_class();
    Spyglass::Rect::define_ruby_class();
    Spyglass::Size::define_ruby_class();
    Spyglass::VideoCapture::define_ruby_class();

    Spyglass::GUI::define_ruby_module();
    Spyglass::GUI::Window::define_ruby_class();
  }
}
