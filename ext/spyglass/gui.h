#ifndef SPYGLASS_GUI_H_
#define SPYGLASS_GUI_H_

#include "spyglass.h"

namespace Spyglass {
  namespace GUI {
    void define_ruby_module();

    static VALUE rb_wait_key(VALUE klass, VALUE timeout);
  }
}

#endif // SPYGLASS_GUI_H_
