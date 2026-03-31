#pragma once
#ifndef __UTILITIES_HPP
    #define __UTILITIES_HPP 1
#endif

#include <cassert>
#include <cstdio>
#include <optional>

// clang-format off
#include <config.hpp>
// clang-format on

#include <QtCore/QFile>
#include <QtCore/QString>

// https://doc.qt.io/qt-6/exceptionsafety.html
// Qt itself does not use C++ exceptions, so it's safe to mark functions with noexcept() unless we use C++ exceptions
// even though none of Qt functions or methods have an explicit noexcept specifier, we assume they won't throw

namespace utilities {

    [[nodiscard]] static inline std::optional<QString> read_qss(const char* const fpath) noexcept {
        // using Qt IO functions so we don't need to marshall plain ascii buffers into QChar buffers by hand
        QFile _stylesheet { fpath };
        if (!_stylesheet.open(QIODeviceBase::OpenModeFlag::ReadOnly | QIODeviceBase::OpenModeFlag::Text)) {
            ::fprintf(stderr, "Error inside %s, failed to open %s, call to QFile::open() failed!\n", __PRETTY_FUNCTION__, fpath);
            return std::nullopt;
        }
        QByteArray _contents = _stylesheet.readAll();
        _stylesheet.close(); // close the file handle
        return QString(_contents);
    }

    [[maybe_unused]] static inline void __attribute__((__always_inline__)) print_build_info() noexcept {
        ::printf("Colorpicker %s, built with Qt %s, using g++ %s\n", COLORPICKER_FULL_VERSION, QT_FULL_VERSION, CXX_FULL_VERSION);
    }

    static inline constexpr double __attribute__((__always_inline__)) gamma(double _colour) noexcept {
        // sRGB is widely used in display devices as opposed to the plain RGB colour channels used commonly in images
        // in sRGB, each colour channel is subjected to this gamma correction
        _colour /= std::numeric_limits<unsigned char>::max(); // scale it to the range (0, 1.0) from (0, 255)
        return (_colour <= 0.0031308) ? 12.92 * _colour : 1.055 * ::pow(_colour, 1 / 2.400) - 0.055;
    }

    static inline constexpr Qt::GlobalColor __attribute__((__always_inline__)) text_colour(
        const double& _red, const double& _green, const double& _blue // this function assumes the inputs will be in the range (0, 255)
    ) noexcept {
        return (0.2126 * gamma(_red) + 0.7152 * gamma(_green) + 0.0722 * gamma(_blue) > 0.700 /* the cutoff */) ? Qt::GlobalColor::black :
                                                                                                                  Qt::GlobalColor::white;
    }

} // namespace utilities
