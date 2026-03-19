#pragma once
#ifndef __COLORPICKER_HPP
    #define __COLORPICKER_HPP           1
    #define __COLORPICKER_VERSION_MAJOR 0
    #define __COLORPICKER_VERSION_MINOR 1
    #define __COLORPICKER_VERSION_PATCH 0
#endif

#define __expand(_number)                          #_number
#define __tostr(_number)                           __expand(_number)
#define __full_version_str(_major, _minor, _patch) (__expand(_major) "." __expand(_minor) "." __expand(_patch))

#include <QtCore/QtGlobal>

static const char* const COLORPICKER_FULL_VERSION { __full_version_str(__COLORPICKER_VERSION_MAJOR, __COLORPICKER_VERSION_MINOR, __COLORPICKER_VERSION_PATCH) };
static const char* const QT_FULL_VERSION { __full_version_str(QT_VERSION_MAJOR, QT_VERSION_MINOR, QT_VERSION_PATCH) };
static const char* const CXX_FULL_VERSION { __full_version_str(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__) };
static const char* const CPP_VERSION { __tostr(__cplusplus) };

[[maybe_unused]] static inline void __print_build_info() noexcept {
    //
}
