#include "window.h"

static VALUE WindowClass;

namespace Spyglass {
  namespace GUI {
    namespace Window {
      void define_ruby_class() {
        // Class definition
        WindowClass = rb_define_class_under(Spyglass::GUI::get_ruby_module(), "Window", rb_cObject);
        rb_define_alloc_func(WindowClass, rb_alloc);
        rb_define_method(WindowClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), 1);

        // Instance methods
        rb_define_method(WindowClass, "hide", RUBY_METHOD_FUNC(rb_hide), 0);
        rb_define_method(WindowClass, "move", RUBY_METHOD_FUNC(rb_move), 2);
        rb_define_method(WindowClass, "on_click", RUBY_METHOD_FUNC(rb_on_click), 0);
        rb_define_method(WindowClass, "on_right_click", RUBY_METHOD_FUNC(rb_on_right_click), 0);
        rb_define_method(WindowClass, "on_move", RUBY_METHOD_FUNC(rb_on_move), 0);
        rb_define_method(WindowClass, "show", RUBY_METHOD_FUNC(rb_show), 1);
        rb_define_method(WindowClass, "title", RUBY_METHOD_FUNC(rb_get_title), 0);
      }

      VALUE get_ruby_class() {
        return WindowClass;
      }

      static VALUE rb_alloc(VALUE self) {
        window_data *window = (window_data *)malloc(sizeof(window_data));
        window->title = NULL;
        return Data_Wrap_Struct(WindowClass, NULL, rb_free, window);
      }

      static void rb_free(window_data *window) {
        if(window->title)
          free(window->title);
        free(window);
      }

      static void _on_mouse_event(int event, int x, int y, int flags, void *obj) {
        window_data *window = (window_data *)obj;
        VALUE self = window->robj;

        VALUE block = 0;
        switch(event) {
          case cv::EVENT_LBUTTONDOWN:
            block = rb_ivar_get(self, rb_intern("@_click_block"));
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


      static VALUE rb_initialize(VALUE self, VALUE title) {
        Check_Type(title, T_STRING);

        window_data *window = SG_GET_WINDOW(self);
        window->title = (char *)malloc(rb_str_length(title) + 1);
        strncpy(window->title, StringValuePtr(title), rb_str_length(title));

        window->robj = self;

        cv::namedWindow(window->title);
        cv::setMouseCallback(window->title, _on_mouse_event, window);
        return self;
      }

      static VALUE rb_hide(VALUE self) {
        window_data *window = SG_GET_WINDOW(self);

        cv::destroyWindow(window->title);
        cv::waitKey(1);

        return self;
      }

      static VALUE rb_on_click(VALUE self) {
        rb_need_block();
        rb_ivar_set(self, rb_intern("@_click_block"), rb_block_proc());
        return self;
      }

      static VALUE rb_on_right_click(VALUE self) {
        rb_need_block();
        rb_ivar_set(self, rb_intern("@_right_click_block"), rb_block_proc());
        return self;
      }

      static VALUE rb_on_move(VALUE self) {
        rb_need_block();
        rb_ivar_set(self, rb_intern("@_mouse_move_block"), rb_block_proc());
        return self;
      }

      static VALUE rb_move(VALUE self, VALUE x, VALUE y) {
        Check_Type(x, T_FIXNUM);
        Check_Type(y, T_FIXNUM);

        window_data *window = SG_GET_WINDOW(self);
        cv::moveWindow(window->title, FIX2INT(x), FIX2INT(y));
        cv::waitKey(1);

        return self;
      }

      static VALUE rb_show(VALUE self, VALUE image) {
        if(!CLASS_OF(image) == Image::get_ruby_class()) {
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
              rb_obj_classname(image));
        }

        cv::Mat *img = SG_GET_IMAGE(image);

        window_data *window = SG_GET_WINDOW(self);
        cv::imshow(window->title, *img);
        cv::waitKey(1);

        return self;
      }

      static VALUE rb_get_title(VALUE self) {
        window_data *window = SG_GET_WINDOW(self);
        return rb_str_new2(window->title);
      }
    }
  }
}
