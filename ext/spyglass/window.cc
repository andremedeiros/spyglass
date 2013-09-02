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
        rb_define_method(WindowClass, "show", RUBY_METHOD_FUNC(rb_show), 1);
        rb_define_method(WindowClass, "title", RUBY_METHOD_FUNC(rb_get_title), 0);
      }

      VALUE get_ruby_class() {
        return WindowClass;
      }

      static VALUE rb_alloc(VALUE self) {
        char *title = NULL;
        return Data_Wrap_Struct(WindowClass, NULL, rb_free, title);
      }

      static void rb_free(char *title) {
        if(title != NULL)
          free(title);
      }

      static VALUE rb_initialize(VALUE self, VALUE title) {
        Check_Type(title, T_STRING);

        char *_title = strdup(StringValueCStr(title));
        cv::namedWindow(_title);
        Data_Set_Struct(self, _title);

        return self;
      }

      static VALUE rb_hide(VALUE self) {
        char *title;
        Data_Get_Struct(self, char, title);

        cv::destroyWindow(title);
        cv::waitKey(1);

        return self;
      }

      static VALUE rb_move(VALUE self, VALUE x, VALUE y) {
        Check_Type(x, T_FIXNUM);
        Check_Type(y, T_FIXNUM);

        char *title;
        Data_Get_Struct(self, char, title);

        cv::moveWindow(title, FIX2INT(x), FIX2INT(y));
        cv::waitKey(1);

        return self;
      }

      static VALUE rb_show(VALUE self, VALUE image) {
        if(!CLASS_OF(image) == Spyglass::Image::get_ruby_class()) {
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Image)",
              rb_obj_classname(image));
        }

        cv::Mat *img = SG_GET_IMAGE(image);

        char *title;
        Data_Get_Struct(self, char, title);

        cv::imshow(title, *img);
        cv::waitKey(1);

        return self;
      }

      static VALUE rb_get_title(VALUE self) {
        char *title;
        Data_Get_Struct(self, char, title);

        return rb_str_new2(title);
      }
    }
  }
}
