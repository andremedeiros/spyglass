#ifndef ROPENCV_GUI_WINDOW_H_
#define ROPENCV_GUI_WINDOW_H_

#include "opencv.h"

namespace OpenCV {
  namespace GUI {
    namespace Window {
      void define_ruby_class();
      VALUE get_ruby_class();
      VALUE rb_opencv_gui_window_alloc(VALUE self);
      void rb_opencv_gui_window_free(char *title);

      VALUE rb_opencv_gui_window_initialize(VALUE self, VALUE title);
      VALUE rb_opencv_gui_window_hide(VALUE self);
      VALUE rb_opencv_gui_window_move(VALUE self, VALUE x, VALUE y);
      VALUE rb_opencv_gui_window_show(VALUE self, VALUE image);
      VALUE rb_opencv_gui_window_get_title(VALUE self);
    }
  }
}

#endif // ROPENCV_GUI_WINDOW_H_
