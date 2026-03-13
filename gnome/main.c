#include <gtk/gtk.h>

#define MAINWINDOW_HEIGHT            330 // height of the application window
#define MAINWINDOW_WIDTH             610 // width of the application window

#define VSPACE_TRACKBARS             40 // vertical separation between subsequent track bars
#define NTRACKBARS                   3  // number of track bars used in the application

#define TRACKBAR_WIDTH               260
#define TRACKBAR_HEIGHT              25
#define TRACKBAR_LEFTPAD             20  // padding between the left end of the track bars and the left edge of the client window
#define TRACKBARGRID_VERTICAL_MARGIN 150 // margin betwen the first track bar and the bottom edge of the title bar

#define TRACKBAR_LABEL_LEFTPAD       20 // padding between the right end of the track bar and the left edge of it's cognate label box
#define TRACKBAR_LABEL_WIDTH         50
#define TRACKBAR_LABEL_HEIGHT        30

#define HEXSTRING_SIZE               20 // max length of the hexadecimal colour code string e.g. "#00AEEF"
#define HEXSTRING_TEXTBOX_WIDTH      120
#define HEXSTRING_TEXTBOX_HEIGHT     30

#define PAGE_UPDOWN_STEP             17 // distance to move the slider on the track bar, when page-up or page-down keys were pressed

static inline GtkWidget* __attribute__((__always_inline__)) new_trackbar() {
    // RGB sliders
    GtkWidget* const trackbar = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 256.0, 1.0);
    gtk_widget_set_halign(trackbar, GTK_ALIGN_FILL);
    gtk_widget_set_valign(trackbar, GTK_ALIGN_CENTER);

    return trackbar;
}

static inline void activate(GtkApplication* const _instance, [[maybe_unused]] const void* const _args) {
    GtkWidget* const main_window =
        gtk_application_window_new(_instance); // draw the window with just a frame, title bar and window controls on screen
    // the GTK_WINDOW macro safely type casts the passed object into a GtkWindow object, at runtime
    // this runtime check ensures that the passed object is an instance of GtkWindow, and will emit a warning if not
    gtk_window_set_title(GTK_WINDOW(main_window), "Colorpicker");
    gtk_window_set_default_size(GTK_WINDOW(main_window), MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);

    gtk_window_set_child(GTK_WINDOW(main_window), r_trackbar);
    gtk_window_present(GTK_WINDOW(main_window));
}

int main(void) { // with GTK, the job of a main() function is creating a GtkApplication object and running it
    // to read more on GNOME application IDs - https://developer.gnome.org/documentation/tutorials/application-id.html
    GtkApplication* instance = gtk_application_new("com.github.anoban.Colorpicker", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(instance, "activate", G_CALLBACK(activate), NULL); // the name of the signal needs to be "activate"
    int status = g_application_run(G_APPLICATION(instance), 0, NULL);
    g_object_unref(instance);
    return status;
}
