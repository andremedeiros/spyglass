#include "window.h"

static VALUE WindowClass;

namespace OpenCV {
  namespace GUI {
    namespace Window {
      void define_ruby_class() {
        // Class definition
        WindowClass = rb_define_class_under(OpenCV::GUI::get_ruby_module(), "Window", rb_cObject);
        rb_define_alloc_func(WindowClass, rb_opencv_gui_window_alloc);
        rb_define_method(WindowClass, "initialize", RUBY_METHOD_FUNC(rb_opencv_gui_window_initialize), 1);

        // Instance methods
        rb_define_method(WindowClass, "hide", RUBY_METHOD_FUNC(rb_opencv_gui_window_hide), 0);
        rb_define_method(WindowClass, "move", RUBY_METHOD_FUNC(rb_opencv_gui_window_move), 2);
        rb_define_method(WindowClass, "show", RUBY_METHOD_FUNC(rb_opencv_gui_window_show), 1);
        rb_define_method(WindowClass, "title", RUBY_METHOD_FUNC(rb_opencv_gui_window_get_title), 0);
      }

      VALUE get_ruby_class() {
        return WindowClass;
      }

      VALUE rb_opencv_gui_window_alloc(VALUE self) {
        char *title = NULL;
        return Data_Wrap_Struct(WindowClass, NULL, rb_opencv_gui_window_free, title);
      }

      void rb_opencv_gui_window_free(char *title) {
        if(title != NULL)
          free(title);
      }

      VALUE rb_opencv_gui_window_initialize(VALUE self, VALUE title) {
        Check_Type(title, T_STRING);

        char *__title = StringValueCStr(title);
        char *_title = (char *)malloc(sizeof(char) * strlen(__title));
        strcpy(_title, __title);
        cv::namedWindow(_title);
        Data_Set_Struct(self, _title);

        return self;
      }

      VALUE rb_opencv_gui_window_hide(VALUE self) {
        char *title;
        Data_Get_Struct(self, char, title);

        cv::destroyWindow(title);
        cv::waitKey(1);
      }

      VALUE rb_opencv_gui_window_move(VALUE self, VALUE x, VALUE y) {
        Check_Type(x, T_FIXNUM);
        Check_Type(y, T_FIXNUM);

        char *title;
        Data_Get_Struct(self, char, title);

        cv::moveWindow(title, FIX2INT(x), FIX2INT(y));
        cv::waitKey(1);
      }

      VALUE rb_opencv_gui_window_show(VALUE self, VALUE image) {
        if(!CLASS_OF(image) == OpenCV::Image::get_ruby_class()) {
          rb_raise(rb_eTypeError, "expected OpenCV::Image");
        }

        cv::Mat *img;
        Data_Get_Struct(image, cv::Mat, img);

        char *title;
        Data_Get_Struct(self, char, title);

        cv::imshow(title, *img);
        cv::waitKey(1);

        return self;
      }

      VALUE rb_opencv_gui_window_get_title(VALUE self) {
        char *title;
        Data_Get_Struct(self, char, title);

        return rb_str_new2(title);
      }
    }
  }
}
