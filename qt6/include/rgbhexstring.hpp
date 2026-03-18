#pragma once
#ifndef __HEXSTRING_HPP
    #define __HEXSTRING_HPP 1
#endif

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

// clang-format off
#include <config.hpp>
// clang-format on

#include <cstdio>
#include <cstring>

class rgbhexstring final : public QLabel {
        //
        Q_OBJECT

    private:
        int  _rslider_value;
        int  _gslider_value;
        int  _bslider_value;
        char _hexstring[configs::hexstring::SIZE];

    public:
        inline rgbhexstring(QWidget* const _parent_window = nullptr) noexcept :
            QLabel(_parent_window), _rslider_value {}, _gslider_value {}, _bslider_value {}, _hexstring {} {
            //
            ::memset(_hexstring, 0, sizeof(_hexstring)); // de we really need this??
        }

        Q_SLOT inline void rslider_moved(int _value) noexcept { _rslider_value = _value; }

        Q_SLOT inline void gslider_moved(int _value) noexcept { }

        Q_SLOT inline void bslider_moved(int _value) noexcept { }

    private:
        inline void __attribute__((__always_inline__)) __update_hexstring() noexcept {
            ::snprintf(_hexstring, sizeof(_hexstring), "#%02X%02X%02X", _rslider_value, _gslider_value, _bslider_value);
        }
};
