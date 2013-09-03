#include "color_space.h"

VALUE ColorSpaceModule;

namespace Spyglass {
  namespace ColorSpace {
    void define_ruby_module() {
      ColorSpaceModule = rb_define_module_under(Spyglass::get_ruby_module(), "ColorSpace");

      // BGR
      rb_define_const(ColorSpaceModule, "BGR_TO_GRAY",  INT2FIX(CV_BGR2GRAY));
      rb_define_const(ColorSpaceModule, "BGR_TO_HLS",   INT2FIX(CV_BGR2HLS));
      rb_define_const(ColorSpaceModule, "BGR_TO_HSV",   INT2FIX(CV_BGR2HSV));
      rb_define_const(ColorSpaceModule, "BGR_TO_YCRCB", INT2FIX(CV_BGR2YCrCb));

      // Gray
      rb_define_const(ColorSpaceModule, "GRAY_TO_BGR", INT2FIX(CV_GRAY2BGR));
      rb_define_const(ColorSpaceModule, "GRAY_TO_RGB", INT2FIX(CV_GRAY2RGB));

      // HLS
      rb_define_const(ColorSpaceModule, "HLS_TO_BGR", INT2FIX(CV_HLS2BGR));
      rb_define_const(ColorSpaceModule, "HLS_TO_RGB", INT2FIX(CV_HLS2RGB));

      // HSV
      rb_define_const(ColorSpaceModule, "HSV_TO_BGR", INT2FIX(CV_HSV2BGR));
      rb_define_const(ColorSpaceModule, "HSV_TO_RGB", INT2FIX(CV_HSV2RGB));

      // RGB
      rb_define_const(ColorSpaceModule, "RGB_TO_GRAY",  INT2FIX(CV_RGB2GRAY));
      rb_define_const(ColorSpaceModule, "RGB_TO_HLS",   INT2FIX(CV_RGB2HLS));
      rb_define_const(ColorSpaceModule, "RGB_TO_HSV",   INT2FIX(CV_RGB2HSV));
      rb_define_const(ColorSpaceModule, "RGB_TO_YCRCB", INT2FIX(CV_RGB2YCrCb));

      // YCrCb
      rb_define_const(ColorSpaceModule, "YCRCB_TO_BGR", INT2FIX(CV_YCrCb2BGR));
      rb_define_const(ColorSpaceModule, "YCRCB_TO_RGB", INT2FIX(CV_YCrCb2RGB));
    }
  }
}
