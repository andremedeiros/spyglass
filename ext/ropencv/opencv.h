#ifndef ROPENCV_OPENCV_H_
#define ROPENCV_OPENCV_H_

#include "ruby.h"

#include "prelude.h"

#include "cascade_classifier.h"
#include "image.h"
#include "point.h"
#include "rect.h"
#include "size.h"
#include "video_capture.h"

#include "gui.h"
#include "window.h"

namespace OpenCV {
  VALUE get_ruby_module();
}

extern "C" void Init_ropencv();

#endif // ROPENCV_OPENCV_H_
