#ifndef SPYGLASS_GUI_WINDOW_H_
#define SPYGLASS_GUI_WINDOW_H_

#include "spyglass.h"

namespace Spyglass {
  namespace GUI {
    namespace Window {
      void define_ruby_class();
      VALUE get_ruby_class();

      static VALUE rb_alloc(VALUE self);
      static void rb_free(char *title);
      static VALUE rb_initialize(VALUE self, VALUE title);
      static VALUE rb_hide(VALUE self);
      static VALUE rb_move(VALUE self, VALUE x, VALUE y);
      static VALUE rb_show(VALUE self, VALUE image);
      static VALUE rb_get_title(VALUE self);
    }
  }
}

#endif // SPYGLASS_GUI_WINDOW_H_
