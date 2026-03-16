#pragma once
#ifndef __CONFIG_HPP
    #define __CONFIG_HPP 1
#endif

#include <colorpicker.hpp>

namespace configs {

    static const char* const APPLICATION_TITLE { "Colorpicker" };

    namespace trackbars {

        static constexpr unsigned long N { 3 };       // number of track bars used in the application
        static constexpr unsigned long VSPACE { 40 }; // vertical separation between subsequent track bars

        static constexpr unsigned long LEFTPAD { 20 }; // padding between the left end of the track bars and the left edge of the client window
        static constexpr unsigned long WIDTH { 260 };
        static constexpr unsigned long HEIGHT { 25 };

        static constexpr unsigned long VERTICAL_MARGIN { 150 }; // margin betwen the first track bar and the bottom edge of the title bar
        static constexpr unsigned long PAGEKEY_STEP { 17 };     // distance to move the slider on the track bar, when page-up or page-down keys were pressed
        static constexpr unsigned long TICK_INTERVAL { 15 };    // interval to position subsequent ticks along the QSlider (trackbar)

        namespace labels {

            // padding between the right end of the track bar and the left edge of it's cognate label box
            static constexpr unsigned long LEFTPAD { 20 };
            static constexpr unsigned long WIDTH { 50 };
            static constexpr unsigned long HEIGHT { 30 };
        }

    } // namespace trackbars

    namespace main_window {

        static constexpr unsigned long HEIGHT { 330 }; // height of the application window
        static constexpr unsigned long WIDTH { 610 };  // width of the application window
    }

    static constexpr unsigned long HEXSTRING_SIZE { 20 }; // max length of the hexadecimal colour code string e.g. "#00AEEF"
    static constexpr unsigned long HEXSTRING_TEXTBOX_WIDTH { 120 };
    static constexpr unsigned long HEXSTRING_TEXTBOX_HEIGHT { 30 };

} // namespace configs
