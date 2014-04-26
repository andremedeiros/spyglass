#ifndef SPYGLASS_WINDOW_H_
#define SPYGLASS_WINDOW_H_

#include "spyglass.h"

typedef struct window_data {
  char *title;
  VALUE robj;
} window_data;

void rb_window_init();

VALUE rb_window_alloc(VALUE self);
void rb_window_free(window_data *window);
VALUE rb_window_initialize(VALUE self, VALUE r_title);
VALUE rb_window_hide(VALUE self);
VALUE rb_window_move(VALUE self, VALUE r_x, VALUE r_y);
VALUE rb_window_on_click(VALUE self);
VALUE rb_window_on_double_click(VALUE self);
VALUE rb_window_on_right_click(VALUE self);
VALUE rb_window_on_move(VALUE self);
VALUE rb_window_show(VALUE self, VALUE r_image);
VALUE rb_window_get_title(VALUE self);

SG_GEN_GET_OBJECT_FUNCTION(SG_GET_WINDOW, window_data);

#endif // SPYGLASS_WINDOW_H_
