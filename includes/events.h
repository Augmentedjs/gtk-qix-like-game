#ifndef EVENTS_H
#define EVENTS_H

#include <gtk/gtk.h>

gboolean on_timeout(gpointer user_data);
gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data);

#endif
