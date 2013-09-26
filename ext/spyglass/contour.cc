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
      rb_define_method(ContourClass, "corners", RUBY_METHOD_FUNC(rb_get_corners), 0);
      rb_define_method(ContourClass, "convex?", RUBY_METHOD_FUNC(rb_is_convex), 0);
      rb_define_method(ContourClass, "rect", RUBY_METHOD_FUNC(rb_get_rect), 0);

      // Aliases
      rb_define_alias(ContourClass, "closed?", "convex?");
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
        if(CLASS_OF(point) != Point::get_ruby_class()) {
          rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Point)",
              rb_obj_classname(point));
        }

        cv::Point *_point = new cv::Point(* SG_GET_POINT(point));
        contour->push_back(_point);
      }

      return self;
    }

    static VALUE rb_get_corners(VALUE self) {
      // This method is a bit of a misnomer in terms of how OpenCV works,
      // seen as it's not a simple getter. It's implemented for ease of
      // use. This code was blatantly based on:
      //
      // http://opencv-code.com/tutorials/automatic-perspective-correction-for-quadrilateral-objects/
      std::vector<cv::Point> contour = to_value_vector(SG_GET_CONTOUR(self));
      cv::approxPolyDP(contour, contour, cv::arcLength(cv::Mat(contour), true) * 0.02, true);
      if (contour.size() != 4) {
        // TODO: Throw exception here?
        std::cout << "The object is not quadrilateral!" << std::endl;
        return Qnil;
      }

      // Get mass center
      cv::Point center(0,0);
      for (int i = 0; i < contour.size(); i++)
        center += contour[i];
      center *= (1. / contour.size());

      // Grab TL, TR, BL, and BR corners.
      std::vector<cv::Point> top, bot;
      for (int i = 0; i < contour.size(); i++) {
        if (contour[i].y < center.y)
          top.push_back(contour[i]);
        else
          bot.push_back(contour[i]);
      }

      cv::Point tl = top[0].x > top[1].x ? top[1] : top[0];
      cv::Point tr = top[0].x > top[1].x ? top[0] : top[1];
      cv::Point bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
      cv::Point br = bot[0].x > bot[1].x ? bot[0] : bot[1];

      contour.clear();
      contour.push_back(tl);
      contour.push_back(tr);
      contour.push_back(br);
      contour.push_back(bl);

      return from_cvpoint_vector(contour);
    }

    static VALUE rb_is_convex(VALUE self) {
      std::vector<cv::Point> contour = to_value_vector(SG_GET_CONTOUR(self));
      std::vector<cv::Point> simplified;
      cv::approxPolyDP(contour, simplified, cv::arcLength(cv::Mat(contour), true) * 0.02, true);

      return cv::isContourConvex(simplified) ? Qtrue : Qfalse;
    }

    static VALUE rb_get_rect(VALUE self) {
      std::vector<cv::Point *> *contour = SG_GET_CONTOUR(self);
      cv::Rect rect = cv::boundingRect(to_value_vector(contour));
      cv::Rect *_rect = new cv::Rect(rect);
      return Rect::from_cvrect(_rect);
    }

    std::vector<cv::Point> to_value_vector(std::vector<cv::Point *> *contour) {
      std::vector<cv::Point> result;
      for(int idx = 0; idx < contour->size(); idx++) {
        cv::Point *point = (*contour)[idx];
        result.push_back(*point);
      }

      return result;
    }

    VALUE from_cvpoint_vector(std::vector<cv::Point> contours) {
      std::vector<cv::Point *> *ctrs = new std::vector<cv::Point *>();

      for(int idx = 0; idx < contours.size(); idx++) {
        ctrs->push_back(new cv::Point(contours[idx]));
      }

      return Data_Wrap_Struct(ContourClass, NULL, rb_free, ctrs);
    }

    VALUE from_contour_vector(std::vector<std::vector<cv::Point> > contours) {
      VALUE ctrs = rb_ary_new2(contours.size());
      for(int idx = 0; idx < contours.size(); idx++) {
        rb_ary_store(ctrs, idx, from_cvpoint_vector(contours[idx]));
      }

      return ctrs;
    }

  }
}
