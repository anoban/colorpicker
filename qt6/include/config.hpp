#pragma once
#ifndef __CONFIG_HPP
    #define __CONFIG_HPP 1
#endif

#include <array>

#include <colorpicker.hpp>
#include <QtCore/QSize>

namespace configs {

    static constexpr unsigned long FONTSIZE { 16 };
    static const char* const       APPLICATION_TITLE { "Colorpicker" };

    enum rgb_offsets : unsigned char { RED, GREEN, BLUE }; // to conveniently index into RGB arrays without using numbers

    namespace titlebar {
        // to index into the array of push buttons our title bar has
        enum button_offsets : unsigned char { MINIMIZE, CLOSE, STAYONTOP, COLOURPICKER, ABOUT };

        static constexpr unsigned long NBUTTONS { 5 };
        static constexpr QSize         BUTTON_SIZE { 36, 36 };
        static constexpr QSize         ICON_SIZE { 30, 30 };
        static constexpr unsigned long WIDTH { 610 }; // we need these two to have matching widths
        static constexpr unsigned long HEIGHT { 32 };
    }

    namespace main_window {
        static constexpr unsigned long HEIGHT { 170 + titlebar::HEIGHT }; // height of the application window
        static constexpr unsigned long WIDTH { titlebar::WIDTH };         // width of the application window, must match with the custom title bar
    }

    namespace sliders {

        static constexpr unsigned long N { 3 };     // number of track bars used in the application
        static constexpr unsigned long HPAD { 20 }; // padding between the left end of the track bars and the left edge of the main window
        static constexpr unsigned long WIDTH { 282 };
        static constexpr unsigned long HEIGHT { 25 };
        static constexpr unsigned long VSPACE_SLIDERS { 44 };            // vertical separation between subsequent track bars
        static constexpr unsigned long VSPACE { 34 + titlebar::HEIGHT }; // margin betwen the first track bar and the top edge of the main window
        // this space should be big enough to accomodate the custom title bar
        static constexpr std::array<const char* const, N> QSS_CLASS_NAMES { "red", "green", "blue" };

        namespace spinboxes {
            // padding between the right end of the track bar and the left edge of it's cognate label box
            static constexpr unsigned long HPAD { 28 };
            static constexpr unsigned long WIDTH { 60 };
            static constexpr unsigned long HEIGHT { 30 };
            static const QString WHITETEXT { "QSpinBox {border: 1px solid transparent; background: rgba(0, 0, 0, 30); color: white; border-radius: 2px;}" };
            static const QString BLACKTEXT { "QSpinBox {border: 1px solid transparent; background: rgba(0, 0, 0, 30); color: black; border-radius: 2px;}" };
        }

    } // namespace sliders

    namespace hexstring {
        static constexpr unsigned long SIZE { 64 }; // max length of the hexadecimal colour code string e.g. "#00AEEF"
        static constexpr unsigned long WIDTH { 140 };
        static constexpr unsigned long HEIGHT { 32 };
        static const QString WHITETEXT { "QLineEdit {border: 1px solid transparent; background: rgba(0, 0, 0, 30); border-radius: 2px; color: white}" };
        static const QString BLACKTEXT { "QLineEdit {border: 1px solid transparent; background: rgba(0, 0, 0, 30); border-radius: 2px; color: black}" };
    }
} // namespace configs
