#include "window.h"

extern VALUE GuiModule;
extern VALUE ImageClass;

VALUE WindowClass = Qnil;

void rb_window_init() {
  // Class definition
  WindowClass = rb_define_class_under(GuiModule, "Window", rb_cObject);
  rb_define_alloc_func(WindowClass, rb_window_alloc);
  rb_define_method(WindowClass, "initialize", RUBY_METHOD_FUNC(rb_window_initialize), 1);

  // Instance methods
  rb_define_method(WindowClass, "hide", RUBY_METHOD_FUNC(rb_window_hide), 0);
  rb_define_method(WindowClass, "move", RUBY_METHOD_FUNC(rb_window_move), 2);
  rb_define_method(WindowClass, "on_click", RUBY_METHOD_FUNC(rb_window_on_click), 0);
  rb_define_method(WindowClass, "on_double_click", RUBY_METHOD_FUNC(rb_window_on_double_click), 0);
  rb_define_method(WindowClass, "on_right_click", RUBY_METHOD_FUNC(rb_window_on_right_click), 0);
  rb_define_method(WindowClass, "on_move", RUBY_METHOD_FUNC(rb_window_on_move), 0);
  rb_define_method(WindowClass, "show", RUBY_METHOD_FUNC(rb_window_show), 1);
  rb_define_method(WindowClass, "title", RUBY_METHOD_FUNC(rb_window_get_title), 0);
}

VALUE
rb_window_alloc(VALUE self) {
  window_data *window = (window_data *)malloc(sizeof(window_data));
  window->title = NULL;
  return Data_Wrap_Struct(WindowClass, NULL, rb_window_free, window);
}

void
rb_window_free(window_data *window) {
  if(window->title)
    free(window->title);
  free(window);
}

void
_on_mouse_event(int event, int x, int y, int flags, void *obj) {
  window_data *window = (window_data *)obj;
  VALUE self = window->robj;

  VALUE block = 0;
  switch(event) {
    case cv::EVENT_LBUTTONDOWN:
      block = rb_ivar_get(self, rb_intern("@_click_block"));
      break;

    case cv::EVENT_LBUTTONDBLCLK:
      block = rb_ivar_get(self, rb_intern("@_double_click_block"));
      break;

    case cv::EVENT_RBUTTONDOWN:
      block = rb_ivar_get(self, rb_intern("@_right_click_block"));
      break;

    case cv::EVENT_MOUSEMOVE:
      block = rb_ivar_get(self, rb_intern("@_mouse_move_block"));
      break;
  }

  if(block == 0 || !RTEST(block))
    return;

  rb_funcall(block, rb_intern("call"), 2, INT2FIX(x), INT2FIX(y));
}

VALUE
rb_window_initialize(VALUE self, VALUE title) {
  Check_Type(title, T_STRING);

  window_data *window = SG_GET_WINDOW(self);
  window->title = (char *)malloc(rb_str_length(title) + 1);
  strncpy(window->title, StringValuePtr(title), rb_str_length(title));

  window->robj = self;

  cv::namedWindow(window->title);
  cv::setMouseCallback(window->title, _on_mouse_event, window);
  return self;
}

VALUE
rb_window_hide(VALUE self) {
  window_data *window = SG_GET_WINDOW(self);

  cv::destroyWindow(window->title);
  cv::waitKey(1);

  return self;
}

VALUE
rb_window_on_click(VALUE self) {
  rb_need_block();
  rb_ivar_set(self, rb_intern("@_click_block"), rb_block_proc());
  return self;
}

VALUE
rb_window_on_double_click(VALUE self) {
  rb_need_block();
  rb_ivar_set(self, rb_intern("@_double_click_block"), rb_block_proc());
  return self;
}

VALUE
rb_window_on_right_click(VALUE self) {
  rb_need_block();
  rb_ivar_set(self, rb_intern("@_right_click_block"), rb_block_proc());
  return self;
}

VALUE
rb_window_on_move(VALUE self) {
  rb_need_block();
  rb_ivar_set(self, rb_intern("@_mouse_move_block"), rb_block_proc());
  return self;
}

VALUE
rb_window_move(VALUE self, VALUE x, VALUE y) {
  Check_Type(x, T_FIXNUM);
  Check_Type(y, T_FIXNUM);

  window_data *window = SG_GET_WINDOW(self);
  cv::moveWindow(window->title, FIX2INT(x), FIX2INT(y));
  cv::waitKey(1);

  return self;
}

VALUE
rb_window_show(VALUE self, VALUE image) {
  if(CLASS_OF(image) != ImageClass) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(image));
  }

  cv::Mat *img = SG_GET_IMAGE(image);

  window_data *window = SG_GET_WINDOW(self);
  cv::imshow(window->title, *img);
  cv::waitKey(1);

  return self;
}

VALUE
rb_window_get_title(VALUE self) {
  window_data *window = SG_GET_WINDOW(self);
  return rb_str_new2(window->title);
}
