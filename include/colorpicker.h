#pragma once
#ifndef __COLORPICKER_H__
    #define __COLORPICKER_H__

    #define _AMD64_             // target architecure
    #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
    #define WIN32_EXTRA_MEAN

                                // Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

// clang-format off
    #include <windef.h>
    #include <resource.h>
    #include <math.h> // min and max macros
    #include <dwmapi.h> // Desktop Window Manager header, implements dark mode features.
    #include <SDKDDKVer.h>
    #include <strsafe.h>
    #include <tchar.h>
    #include <Windows.h>
    #include <wingdi.h>
    #include <WinUser.h>
    #include <CommCtrl.h>
// clang-format on

    #pragma comment(lib, "Dwmapi.lib")
    #pragma comment(lib, "Gdi32.lib")
    #pragma comment(lib, "User32.lib")

// prototypes of procs in handlers.cpp

LRESULT CALLBACK ScrollHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // !__COLORPICKER_H__
