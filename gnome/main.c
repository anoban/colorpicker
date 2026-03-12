#include <gtk/gtk.h>

int main(void) {
    GtkApplication* instance = gtk_application_new("com.github.anoban.Colorpicker", G_APPLICATION_DEFAULT_FLAGS);
    int             status   = g_application_run(G_APPLICATION(instance), 0, NULL);
    g_object_unref(instance);
    return status;
}
