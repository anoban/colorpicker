#pragma once
#ifndef __COLORPICKER_H__
    #define __COLORPICKER_H__
    #define _AMD64_
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN

// #define _WIN32_IE_ 0x0600

// clang-format off
    #include <windef.h>
    #include <resource.h>
    #include <dwmapi.h> // Desktop Window Manager header, implements dark mode features.
    #include <SDKDDKVer.h>
    #include <strsafe.h>
    #include <string.h>
    #include <wingdi.h>
    #include <WinUser.h>
    #include <windowsx.h>
    #include <CommCtrl.h>
    #include <math.h>
// clang-format on

    #pragma comment(lib, "Dwmapi.lib")
    #pragma comment(lib, "Gdi32.lib")
    #pragma comment(lib, "User32.lib")
    #pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WindowHandler(_In_ HWND hParentWindow, _In_ const UINT message, _In_ const WPARAM wParam, _In_ const LPARAM lParam);

#endif // !__COLORPICKER_H__
