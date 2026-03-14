#pragma once
#ifndef __CONFIG_HPP
    #define __CONFIG_HPP 1
#endif

static constexpr unsigned long MAINWINDOW_HEIGHT { 330 }; // height of the application window
static constexpr unsigned long MAINWINDOW_WIDTH { 610 };  // width of the application window

static constexpr unsigned long VSPACE_TRACKBARS { 40 }; // vertical separation between subsequent track bars
static constexpr unsigned long NTRACKBARS { 3 };        // number of track bars used in the application

static constexpr unsigned long TRACKBAR_WIDTH { 260 };
static constexpr unsigned long TRACKBAR_HEIGHT { 25 };
static constexpr unsigned long TRACKBAR_LEFTPAD { 20 };              // padding between the left end of the track bars and the left edge of the client window
static constexpr unsigned long TRACKBARGRID_VERTICAL_MARGIN { 150 }; // margin betwen the first track bar and the bottom edge of the title bar

static constexpr unsigned long TRACKBAR_LABEL_LEFTPAD { 20 }; // padding between the right end of the track bar and the left edge of it's cognate label box
static constexpr unsigned long TRACKBAR_LABEL_WIDTH { 50 };
static constexpr unsigned long TRACKBAR_LABEL_HEIGHT { 30 };

static constexpr unsigned long HEXSTRING_SIZE { 20 }; // max length of the hexadecimal colour code string e.g. "#00AEEF"
static constexpr unsigned long HEXSTRING_TEXTBOX_WIDTH { 120 };
static constexpr unsigned long HEXSTRING_TEXTBOX_HEIGHT { 30 };

static constexpr unsigned longPAGE_UPDOWN_STEP { 17 }; // distance to move the slider on the track bar, when page-up or page-down keys were pressed

static const char* const APPLICATION_TITLE { "Colorpicker" };
