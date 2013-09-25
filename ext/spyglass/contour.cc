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
      // use. This code was blatantly stolen from here:
      //
      // http://opencv-code.com/tutorials/automatic-perspective-correction-for-quadrilateral-objects/
 
      // First, we get the contour and the rect so that we can create an
      // image from it and draw it.
      std::vector<cv::Point> contour = to_value_vector(SG_GET_CONTOUR(self));
      std::vector<std::vector<cv::Point> > contours;
      contours.push_back(contour);

      // Create a blank image where the contour for the existing object would
      // fit, and give it 20 pixels of extra space.
      cv::Rect rect = cv::boundingRect(contour);
      cv::Mat img(rect.y + rect.height + 50, rect.x + rect.width + 50, CV_8UC1);
      img.setTo(cv::Scalar(0, 0, 0));

      // Draw the contour, blur it, and canny it.
      cv::drawContours(img, contours, 0, cv::Scalar(255, 255, 255), CV_FILLED);
      cv::Canny(img, img, 100, 100, 3);

      // Detect lines
      std::vector<cv::Vec4i> lines;
      cv::HoughLinesP(img, lines, 1, CV_PI/180, 70, 30, 10);

      // Calculate intersection points amongst lines
      std::vector<cv::Point> corners;
      for (int i = 0; i < lines.size(); i++) {
        for (int j = i+1; j < lines.size(); j++) {
          cv::Vec4i a = lines[i];
          cv::Vec4i b = lines[j];
          cv::Point pt;

          int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
          int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
          if (float d = ((float)(x1-x2) * (y3-y4)) - ((y1-y2) * (x3-x4))) {
            pt.x = ((x1*y2 - y1*x2) * (x3-x4) - (x1-x2) * (x3*y4 - y3*x4)) / d;
            pt.y = ((x1*y2 - y1*x2) * (y3-y4) - (y1-y2) * (x3*y4 - y3*x4)) / d;
          } else {
            pt.x = -1;
            pt.y = -1;
          }

          if (pt.x >= 0 && pt.y >= 0)
            corners.push_back(pt);
        }
      }

      std::vector<cv::Point> approx;
      cv::approxPolyDP(cv::Mat(corners), approx, cv::arcLength(cv::Mat(corners), true) * 0.02, true);
      if (approx.size() != 4) {
        // TODO: Throw exception here?
        std::cout << "The object is not quadrilateral!" << std::endl;
        return Qnil;
      }

      // Get mass center
      cv::Point center(0,0);
      for (int i = 0; i < corners.size(); i++)
        center += corners[i];
      center *= (1. / corners.size());

      // Grab TL, TR, BL, and BR corners.
      std::vector<cv::Point> top, bot;
      for (int i = 0; i < corners.size(); i++) {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
      }

      cv::Point tl = top[0].x > top[1].x ? top[1] : top[0];
      cv::Point tr = top[0].x > top[1].x ? top[0] : top[1];
      cv::Point bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
      cv::Point br = bot[0].x > bot[1].x ? bot[0] : bot[1];

      corners.clear();
      corners.push_back(tl);
      corners.push_back(tr);
      corners.push_back(br);
      corners.push_back(bl);

      return from_cvpoint_vector(corners);
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
