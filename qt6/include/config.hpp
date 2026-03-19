#pragma once
#ifndef __CONFIG_HPP
    #define __CONFIG_HPP 1
#endif

#include <colorpicker.hpp>

namespace configs {

    static const char* const APPLICATION_TITLE { "Colorpicker" };

    namespace main_window {

        static constexpr unsigned long HEIGHT { 170 }; // height of the application window
        static constexpr unsigned long WIDTH { 610 };  // width of the application window
    }

    namespace trackbars {

        static constexpr unsigned long N { 3 };       // number of track bars used in the application
        static constexpr unsigned long VSPACE { 44 }; // vertical separation between subsequent track bars
        static constexpr unsigned long PAD { 20 };    // padding between the left end of the track bars and the left edge of the main window
        static constexpr unsigned long WIDTH { 282 };
        static constexpr unsigned long HEIGHT { 25 };
        static constexpr unsigned long VERTICAL_MARGIN { 34 }; // margin betwen the first track bar and the bottom edge of the title bar

        namespace labels {

            // padding between the right end of the track bar and the left edge of it's cognate label box
            static constexpr unsigned long PAD { 28 };
            static constexpr unsigned long WIDTH { 60 };
            static constexpr unsigned long HEIGHT { 30 };
        }

    } // namespace trackbars

    namespace hexstring {

        static constexpr unsigned long SIZE { 64 }; // max length of the hexadecimal colour code string e.g. "#00AEEF"
        static constexpr unsigned long WIDTH { 140 };
        static constexpr unsigned long HEIGHT { 30 };

    }

} // namespace configs
