#include "gui.h"

extern VALUE rb_mSpyglass;

VALUE rb_mGUI = Qnil;

void
rb_gui_init() {
  rb_mGUI = rb_define_module_under(rb_mSpyglass, "GUI");

  // Module methods
  rb_define_singleton_method(rb_mGUI, "wait_key", RUBY_METHOD_FUNC(rb_gui_wait_key), 1);
}

VALUE
rb_gui_wait_key(VALUE klass, VALUE r_timeout) {
  Check_Type(r_timeout, T_FIXNUM);

  int timeout = FIX2INT(r_timeout);
  int key = cv::waitKey(timeout);

  return INT2FIX(key);
}
