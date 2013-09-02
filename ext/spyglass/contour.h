#ifndef SPYGLASS_CONTOUR_H_
#define SPYGLASS_CONTOUR_H_

#include "spyglass.h"

namespace Spyglass {
  namespace Contour {
    void define_ruby_class();
    VALUE get_ruby_class();

    static VALUE rb_alloc(VALUE self);
    static void rb_free(std::vector<cv::Point *> *contour);
    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self);
    static VALUE rb_get_rect(VALUE self);

    std::vector<cv::Point> to_value_vector(std::vector<cv::Point *> *contour);
  }
}

#endif // SPYGLASS_CONTOUR_H_
