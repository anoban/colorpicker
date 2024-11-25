#pragma once
#define _AMD64_
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN

// clang-format off
    #include <windef.h>
    #include <dwmapi.h> // Desktop Window Manager header, implements dark mode features.
    #include <strsafe.h>
    #include <wingdi.h>
    #include <WinUser.h>
    #include <CommCtrl.h>
// clang-format on

#include <limits.h>
#include <math.h>
#include <resource.h> // project header
#include <string.h>

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WindowHandler(_In_ HWND hParentWindow, _In_ const UINT message, _In_ const WPARAM wParam, _In_ const LPARAM lParam);
