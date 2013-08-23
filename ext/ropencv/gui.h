#ifndef ROPENCV_GUI_H_
#define ROPENCV_GUI_H_

#include "opencv.h"

namespace OpenCV {
  namespace GUI {
    void define_ruby_module();
    VALUE get_ruby_module();
  }
}

#endif
