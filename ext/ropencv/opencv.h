#ifndef ROPENCV_OPENCV_H_
#define ROPENCV_OPENCV_H_

#include "ruby.h"

#include "image.h"
#include "point.h"
#include "rect.h"
#include "size.h"

namespace OpenCV {
  VALUE rb_opencv_module();
}

extern "C" void Init_ropencv();

#endif // ROPENCV_OPENCV_H_
