#include "spyglass.h"

VALUE SpyglassModule = Qnil;

void
Init_spyglass() {
  SpyglassModule = rb_define_module("Spyglass");

  rb_cs_init();       // Module: ColorSpace
  rb_bgs_init();      // Class cluster: Background Subtractors
  rb_cc_init();       // Class: CascadeClassifier
  rb_color_init();    // Class: Color
  rb_contour_init();  // Class: Contour
  rb_img_init();      // Class: Image
  rb_point_init();    // Class: Point
  rb_rect_init();     // Class: Rect
  rb_size_init();     // Class: Size
  rb_cap_init();      // Class: VideoCapture

  rb_gui_init();      // Module: GUI
  rb_window_init();   // Class: GUI::Window
}
