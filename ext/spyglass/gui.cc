#include "gui.h"

extern VALUE SpyglassModule;

VALUE GuiModule = Qnil;

void
rb_gui_init() {
  GuiModule = rb_define_module_under(SpyglassModule, "GUI");

  // Module methods
  rb_define_singleton_method(GuiModule, "wait_key", RUBY_METHOD_FUNC(rb_gui_wait_key), 1);
}

VALUE
rb_gui_wait_key(VALUE klass, VALUE timeout) {
  Check_Type(timeout, T_FIXNUM);

  int msecs = FIX2INT(timeout);
  int key = cv::waitKey(msecs);

  return INT2FIX(key);
}
