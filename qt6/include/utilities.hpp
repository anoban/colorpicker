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

    static inline double constexpr __attribute__((__always_inline__)) rgb_to_greyscale(const double& _red, const double& _green, const double& _blue) noexcept {
        // pixel.rgbBlue * 0.299L + pixel.rgbGreen * 0.587L + pixel.rgbRed * 0.114L
        return _red * 0.114 + _green * 0.587 + _blue * 0.299; // NOLINT(readability-magic-numbers)
    }

} // namespace utilities
