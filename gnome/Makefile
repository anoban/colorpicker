COMPILER = /usr/bin/gcc

GTK4_INCLUDE_PATHS = -I/usr/include/gtk-4.0 -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/harfbuzz -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glycin-2 -I/usr/include/cairo -I/usr/include/libxml2 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/pixman-1 -I/usr/include/graphene-1.0 -I/usr/lib64/graphene-1.0/include -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/sysprof-6 # pkg-config --cflags gtk4

GTK4_LIBS = -lgtk-4 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -lgdk_pixbuf-2.0 -lcairo-gobject -lcairo -lvulkan -lgraphene-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 # pkg-config --libs gtk4

GTK4_DEFINES = -DWITH_GZFILEOP

DEBUG = -DDEBUG -D_DEBUG -O0 -g3

NODEBUG = -DNDEBUG -D_NDEBUG -O3 -g0

# -pthread was part of the pkg-config --cflags gtk4 output
CFALGS = -Wall -Wextra -Wdeprecated-declarations -std=c23 -march=tigerlake -mavx512f -ffast-math -mprefer-vector-width=512 -pthread

TARGET = main

build:
	$(COMPILER) $(TARGET).c $(GTK4_DEFINES) $(NODEBUG) $(GTK4_INCLUDE_PATHS) $(GTK4_LIBS) $(CFALGS) -o colorpicker.out

clean:
	rm -rf ./*.out
	rm -rf ./*.o
