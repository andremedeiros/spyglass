#include "window.h"

extern VALUE rb_mGUI;
extern VALUE rb_cImage;

VALUE rb_cWindow = Qnil;

void rb_window_init() {
  // Class definition
  rb_cWindow = rb_define_class_under(rb_mGUI, "Window", rb_cObject);
  rb_define_alloc_func(rb_cWindow, rb_window_alloc);
  rb_define_method(rb_cWindow, "initialize", RUBY_METHOD_FUNC(rb_window_initialize), 1);

  // Instance methods
  rb_define_method(rb_cWindow, "hide", RUBY_METHOD_FUNC(rb_window_hide), 0);
  rb_define_method(rb_cWindow, "move", RUBY_METHOD_FUNC(rb_window_move), 2);
  rb_define_method(rb_cWindow, "on_click", RUBY_METHOD_FUNC(rb_window_on_click), 0);
  rb_define_method(rb_cWindow, "on_double_click", RUBY_METHOD_FUNC(rb_window_on_double_click), 0);
  rb_define_method(rb_cWindow, "on_right_click", RUBY_METHOD_FUNC(rb_window_on_right_click), 0);
  rb_define_method(rb_cWindow, "on_move", RUBY_METHOD_FUNC(rb_window_on_move), 0);
  rb_define_method(rb_cWindow, "show", RUBY_METHOD_FUNC(rb_window_show), 1);
  rb_define_method(rb_cWindow, "title", RUBY_METHOD_FUNC(rb_window_get_title), 0);
}

VALUE
rb_window_alloc(VALUE self) {
  window_data *window = (window_data *)malloc(sizeof(window_data));
  window->title = NULL;
  return Data_Wrap_Struct(rb_cWindow, NULL, rb_window_free, window);
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
rb_window_initialize(VALUE self, VALUE r_title) {
  Check_Type(r_title, T_STRING);

  window_data *window = SG_GET_WINDOW(self);
  window->title = (char *)malloc(rb_str_length(r_title) + 1);
  strncpy(window->title, StringValuePtr(r_title), rb_str_length(r_title));

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
rb_window_move(VALUE self, VALUE r_x, VALUE r_y) {
  Check_Type(r_x, T_FIXNUM);
  Check_Type(r_y, T_FIXNUM);

  window_data *window = SG_GET_WINDOW(self);
  cv::moveWindow(window->title, FIX2INT(r_x), FIX2INT(r_y));
  cv::waitKey(1);

  return self;
}

VALUE
rb_window_show(VALUE self, VALUE r_image) {
  if(CLASS_OF(r_image) != rb_cImage) {
    rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
        rb_obj_classname(r_image));
  }

  cv::Mat *img = SG_GET_IMAGE(r_image);

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
