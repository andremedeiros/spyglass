#ifndef SPYGLASS_CASCADE_CLASSIFIER_H_
#define SPYGLASS_CASCADE_CLASSIFIER_H_

#include "spyglass.h"

void rb_cc_init();
VALUE rb_cc_alloc(VALUE self);
void rb_cc_free(cv::CascadeClassifier *classifier);
VALUE rb_cc_initialize(VALUE self, VALUE src);
VALUE rb_cc_detect(int argc, VALUE *argv, VALUE self);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_CLASSIFIER, cv::CascadeClassifier);

#endif // SPYGLASS_CASCADE_CLASSIFIER_H_
