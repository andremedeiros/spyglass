#include "contour.h"

static VALUE ContourClass;

namespace Spyglass {
  namespace Contour {
    void define_ruby_class() {
      // Class definition
      ContourClass = rb_define_class_under(Spyglass::get_ruby_module(), "Contour", rb_cObject);
      rb_define_alloc_func(ContourClass, rb_alloc);
      rb_define_method(ContourClass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);

      // Instance methods
      rb_define_method(ContourClass, "rect", RUBY_METHOD_FUNC(rb_get_rect), 0);
    }

    VALUE get_ruby_class() {
      return ContourClass;
    }

    static VALUE rb_alloc(VALUE self) {
      std::vector<cv::Point *> *contour = new std::vector<cv::Point *>();
      return Data_Wrap_Struct(ContourClass, NULL, rb_free, contour);
    }

    static void rb_free(std::vector<cv::Point *> *contour) {
      contour->clear();
      delete contour;
    }

    static VALUE rb_initialize(int argc, VALUE *argv, VALUE self) {
      VALUE points;
      rb_scan_args(argc, argv, "01", &points);

      if(!RTEST(points))
        return self;

      Check_Type(points, T_ARRAY);

      std::vector<cv::Point *> *contour = SG_GET_CONTOUR(self);
      for(int idx = 0; idx < RARRAY_LEN(points); idx++) {
        VALUE point = rb_ary_entry(points, idx);
        if(CLASS_OF(point) != Spyglass::Point::get_ruby_class()) {
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Point)",
              rb_obj_classname(point));
        }

        cv::Point *_point = new cv::Point(* SG_GET_POINT(point));
        contour->push_back(_point);
      }

      return self;
    }

    static VALUE rb_get_rect(VALUE self) {
      std::vector<cv::Point *> *contour = SG_GET_CONTOUR(self);
      cv::Rect rect = cv::boundingRect(to_value_vector(contour));
      cv::Rect *_rect = new cv::Rect(rect);
      return Spyglass::Rect::rect_from_cvrect(_rect);
    }

    std::vector<cv::Point> to_value_vector(std::vector<cv::Point *> *contour) {
      std::vector<cv::Point> result;
      for(int idx = 0; idx < contour->size(); idx++) {
        cv::Point *point = (*contour)[idx];
        result.push_back(*point);
      }

      return result;
    }
    
    VALUE from_point_vector(std::vector<cv::Point> contours) {
      std::vector<cv::Point *> *ctrs = new std::vector<cv::Point *>();
      
      for(int idx = 0; idx < contours.size(); idx++) {
        ctrs->push_back(new cv::Point(contours[idx]));
      }
      
      return Data_Wrap_Struct(ContourClass, NULL, rb_free, ctrs);
    }
    
    VALUE from_contour_vector(std::vector<std::vector<cv::Point> > contours) {
      VALUE ctrs = rb_ary_new2(contours.size());
      for(int idx = 0; idx < contours.size(); idx++) {
        rb_ary_store(ctrs, idx, from_point_vector(contours[idx]));
      }
      
      return ctrs;
    }
    
  }
}
