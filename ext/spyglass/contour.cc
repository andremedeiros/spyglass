#include "contour.h"

extern VALUE rb_mSpyglass;
extern VALUE rb_cPoint;

VALUE rb_cContour = Qnil;

void
rb_contour_init() {
  // Class definition
  rb_cContour = rb_define_class_under(rb_mSpyglass, "Contour", rb_cObject);
  rb_define_alloc_func(rb_cContour, rb_contour_alloc);
  rb_define_method(rb_cContour, "initialize", RUBY_METHOD_FUNC(rb_contour_initialize), -1);

  // Instance methods
  rb_define_method(rb_cContour, "corners", RUBY_METHOD_FUNC(rb_contour_get_corners), 0);
  rb_define_method(rb_cContour, "convex?", RUBY_METHOD_FUNC(rb_contour_is_convex), 0);
  rb_define_method(rb_cContour, "rect", RUBY_METHOD_FUNC(rb_contour_get_rect), 0);

  // Aliases
  rb_define_alias(rb_cContour, "closed?", "convex?");
}

VALUE
rb_contour_alloc(VALUE self) {
  std::vector<cv::Point *> *contour = new std::vector<cv::Point *>();
  return Data_Wrap_Struct(rb_cContour, NULL, rb_contour_free, contour);
}

void
rb_contour_free(std::vector<cv::Point *> *contour) {
  contour->clear();
  delete contour;
}

VALUE
rb_contour_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE r_points;
  rb_scan_args(argc, argv, "01", &r_points);

  if(!RTEST(r_points))
    return self;

  Check_Type(r_points, T_ARRAY);

  std::vector<cv::Point *> *contour = SG_GET_CONTOUR(self);
  for(int idx = 0; idx < RARRAY_LEN(r_points); idx++) {
    VALUE r_point = rb_ary_entry(r_points, idx);
    if(CLASS_OF(r_point) != rb_cPoint) {
      rb_raise(rb_eTypeError, "wrong argument type %s (expected Spyglass::Point)",
          rb_obj_classname(r_point));
    }

    cv::Point *point = new cv::Point(* SG_GET_POINT(r_point));
    contour->push_back(point);
  }

  return self;
}

VALUE
rb_contour_get_corners(VALUE self) {
  // This method is a bit of a misnomer in terms of how OpenCV works,
  // seen as it's not a simple getter. It's implemented for ease of
  // use. This code was blatantly based on:
  //
  // http://opencv-code.com/tutorials/automatic-perspective-correction-for-quadrilateral-objects/
  std::vector<cv::Point> contour = cv_contour_to_cv_points(SG_GET_CONTOUR(self));
  cv::approxPolyDP(contour, contour, cv::arcLength(cv::Mat(contour), true) * 0.02, true);
  if (contour.size() != 4) {
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

  return cv_points_to_rb_vector(contour);
}

VALUE
rb_contour_is_convex(VALUE self) {
  std::vector<cv::Point> contour = cv_contour_to_cv_points(SG_GET_CONTOUR(self));
  std::vector<cv::Point> simplified;
  cv::approxPolyDP(contour, simplified, cv::arcLength(cv::Mat(contour), true) * 0.02, true);

  return cv::isContourConvex(simplified) ? Qtrue : Qfalse;
}

VALUE
rb_contour_get_rect(VALUE self) {
  std::vector<cv::Point *> *contour = SG_GET_CONTOUR(self);
  cv::Rect rect = cv::boundingRect(cv_contour_to_cv_points(contour));
  cv::Rect *_rect = new cv::Rect(rect);
  return cv_rect_to_rb_rect(_rect);
}

std::vector<cv::Point>
cv_contour_to_cv_points(std::vector<cv::Point *> *contour) {
  std::vector<cv::Point> result;
  for(int idx = 0; idx < contour->size(); idx++) {
    cv::Point *point = (*contour)[idx];
    result.push_back(*point);
  }

  return result;
}

VALUE
cv_points_to_rb_vector(std::vector<cv::Point> contours) {
  std::vector<cv::Point *> *ctrs = new std::vector<cv::Point *>();

  for(int idx = 0; idx < contours.size(); idx++) {
    ctrs->push_back(new cv::Point(contours[idx]));
  }

  return Data_Wrap_Struct(rb_cContour, NULL, rb_contour_free, ctrs);
}

VALUE
cv_contours_to_rb_contours(std::vector<std::vector<cv::Point> > contours) {
  VALUE ctrs = rb_ary_new2(contours.size());
  for(int idx = 0; idx < contours.size(); idx++) {
    rb_ary_store(ctrs, idx, cv_points_to_rb_vector(contours[idx]));
  }

  return ctrs;
}
