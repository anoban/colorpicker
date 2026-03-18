#pragma once
#ifndef __HEXSTRING_HPP
    #define __HEXSTRING_HPP 1
#endif

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

// clang-format off
#include <config.hpp>
// clang-format on

#include <cstring>

class rgbhexstring final : public QLineEdit {
        Q_OBJECT

    private:
        int     _rslider_value;
        int     _gslider_value;
        int     _bslider_value;
        QString _hexstring; // QString is equivalent to std::wstring

    public:
        inline rgbhexstring(QWidget* const _parent_window = nullptr) noexcept :
            QLineEdit(_parent_window), _rslider_value {}, _gslider_value {}, _bslider_value {}, _hexstring {} {
            //
            _hexstring.resize(configs::hexstring::SIZE);
        }

        Q_SLOT inline void rslider_moved(int _value) noexcept {
            _rslider_value = _value;
            __update_hexstring();
        }

        Q_SLOT inline void gslider_moved(int _value) noexcept {
            _gslider_value = _value;
            __update_hexstring();
        }

        Q_SLOT inline void bslider_moved(int _value) noexcept {
            _bslider_value = _value;
            __update_hexstring();
        }

    private:
        inline void __attribute__((__always_inline__)) __update_hexstring() noexcept {
            // static_assert(sizeof(QChar) == sizeof(wchar_t));                   // QChar is equivalent to unsigned short
            // the above requires -fshort-wchar compiler flag
            // by default, wchar_t is 4 bytes on linux??? not 2 bytes
            ::memset(_hexstring.data(), 0, sizeof(QChar) * _hexstring.size()); // clean up the buffer before every write
            // ::swprintf(reinterpret_cast<wchar_t*>(_hexstring.data()), _hexstring.size(), L"#%02X%02X%02X", _rslider_value, _gslider_value, _bslider_value);
            // https://stackoverflow.com/questions/4784155/how-to-format-a-qstring
            // even if we use two byte wchar_t s, we don't know that there's alternative implementations for all the stdio.h functions to handle 2 byte wchar_t s????
            _hexstring.asprintf("#%02X%02X%02X", _rslider_value, _gslider_value, _bslider_value); // seems the most sensible way to accomplish this
            setText(_hexstring);
            ::puts(_hexstring.toStdString().c_str());
        }
};
