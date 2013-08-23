#include "gui.h"

static VALUE GuiModule;

namespace OpenCV {
  namespace GUI {
    void define_ruby_module() {
      GuiModule = rb_define_module_under(OpenCV::get_ruby_module(), "GUI");
    }

    VALUE get_ruby_module() {
      return GuiModule;
    }
  }
}
