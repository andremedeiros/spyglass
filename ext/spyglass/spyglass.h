#ifndef SPYGLASS_SPYGLASS_H_
#define SPYGLASS_SPYGLASS_H_

#include <ruby.h>

#include <opencv2/opencv.hpp>

#include "prelude.h"
#include "background_subtractor.h"
#include "cascade_classifier.h"
#include "contour.h"
#include "image.h"
#include "point.h"
#include "rect.h"
#include "size.h"
#include "video_capture.h"

#include "gui.h"
#include "window.h"

namespace Spyglass {
  VALUE get_ruby_module();
}

extern "C" void Init_spyglass();

#endif // SPYGLASS_SPYGLASS_H_
