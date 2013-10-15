#ifndef SPYGLASS_WINDOW_H_
#define SPYGLASS_WINDOW_H_

#include "spyglass.h"

namespace Spyglass {

  typedef struct window_data {
    char *title;
    VALUE robj;
  } window_data;

  SG_GEN_GET_OBJECT_FUNCTION(SG_GET_WINDOW, window_data);

  namespace GUI {
    namespace Window {
      void define_ruby_class();
      VALUE get_ruby_class();

      static VALUE rb_alloc(VALUE self);
      static void rb_free(window_data *window);
      static VALUE rb_initialize(VALUE self, VALUE title);
      static VALUE rb_hide(VALUE self);
      static VALUE rb_move(VALUE self, VALUE x, VALUE y);
      static VALUE rb_on_click(VALUE self);
      static VALUE rb_on_right_click(VALUE self);
      static VALUE rb_on_move(VALUE self);
      static VALUE rb_show(VALUE self, VALUE image);
      static VALUE rb_get_title(VALUE self);
    }
  }
}


#endif // SPYGLASS_WINDOW_H_
