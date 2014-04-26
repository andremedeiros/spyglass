#ifndef SPYGLASS_GUI_H_
#define SPYGLASS_GUI_H_

#include "spyglass.h"

void rb_gui_init();
VALUE rb_gui_wait_key(VALUE klass, VALUE timeout);

#endif // SPYGLASS_GUI_H_
