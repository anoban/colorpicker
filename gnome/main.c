#include <gtk/gtk.h>

#define MAINWINDOW_HEIGHT 330 // height of the application window
#define MAINWINDOW_WIDTH  610 // width of the application window

static inline void activate(GtkApplication* const _instance, void* const _args) {
    GtkWidget* const window = gtk_application_window_new(_instance);
    gtk_window_set_title(GTK_WINDOW(window), "Colorpicker");
    gtk_window_set_default_size(GTK_WINDOW(window), MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
    gtk_window_present(GTK_WINDOW(window));
}

int main(void) {
    // to read more on GNOME application IDs - https://developer.gnome.org/documentation/tutorials/application-id.html
    GtkApplication* instance = gtk_application_new("com.github.anoban.Colorpicker", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(instance, "activate", G_CALLBACK(activate), NULL); // the name of the signal needs to be "activate"
    int status = g_application_run(G_APPLICATION(instance), 0, NULL);
    g_object_unref(instance);
    return status;
}
