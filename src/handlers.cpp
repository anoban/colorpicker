#include <colorpicker.hpp>

extern HINSTANCE                               hInst;
extern INT32                                   idFocus;
extern WNDPROC                                 oldScroll[3];

[[nodiscard, msvc::noinline]] LRESULT CALLBACK ScrollHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    INT64 iId = ::GetWindowLongPtrW(hWnd, GWLP_ID);
    switch (message) {
        case WM_KEYDOWN :
            if (wParam == VK_TAB) ::SetFocus(::GetDlgItem(::GetParent(hWnd), (iId + (::GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));
            break;
        case WM_SETFOCUS : idFocus = iId; break;
        default          : break;
    }
    return ::CallWindowProcW(oldScroll[iId], hWnd, message, wParam, lParam);
}

[[nodiscard, msvc::noinline]] LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF crPrim[3] { RGB(32, 32, 32), RGB(32, 32, 32), RGB(32, 32, 32) };
    static COLORREF crTitleBar { RGB(32, 32, 32) };
    static HBRUSH   hBrush[3] {}, hStaticBrush {};
    static HWND     hwndScroll[3] {} /* scroll bars */, hwndLabel[3] {} /* labels */, hwndValue[3] {} /* label texts */, hwndRect {};
    static HWND     hwndTextBox {};
    static INT32    color[3] {}, cyChar {};
    static RECT     rcColor {};
    static constexpr const WCHAR* wszColorLabel[] { L"Red", L"Green", L"Blue" };
    static WCHAR                  pswzHexColour[8] {};  // needs to be in the static memory since these are used even wehn the callback isn't invoked
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-logfonta
    static constexpr LOGFONT      lfFontAttrs {};
    static const HFONT            hFont { CreateFontIndirectW(&lfFontAttrs) };
    HINSTANCE                     hInstance {};
    INT64                         i {}, cxClient {}, cyClient {};
    static WCHAR                  wszBuffer[8] {};   // needs to be in the static memory since these are used even wehn the callback isn't invoked
    static constexpr BOOL         bUseDarkMode = TRUE; // make the title bar

    switch (message) {
        case WM_CREATE :
            {
                hInstance = reinterpret_cast<HINSTANCE>(::GetWindowLongPtrW(hWnd, GWLP_HINSTANCE));
                ::SetMenu(hWnd, nullptr);              // hide the menu bar

                // https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
                ::DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &bUseDarkMode, sizeof(BOOL));
                ::DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crTitleBar, sizeof(COLORREF));

                hwndRect = ::CreateWindowExW(
                    0L,
                    L"static",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                    0,
                    0,
                    0,
                    0,
                    hWnd,
                    reinterpret_cast<HMENU>(9),
                    hInstance,
                    nullptr
                );

                hwndTextBox = ::CreateWindowExW(
                    WS_EX_CLIENTEDGE,
                    L"edit",
                    nullptr,
                    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED,
                    300,
                    10,
                    100,
                    22,
                    hWnd,
                    reinterpret_cast<HMENU>(10),
                    hInstance,
                    nullptr
                );

                for (i = 0; i < 3; ++i) {
                    hwndScroll[i] = ::CreateWindowExW(
                        0L,
                        L"scrollbar",
                        nullptr,
                        WS_CHILD | WS_VISIBLE | SBS_VERT | WS_TABSTOP,
                        0,
                        0,
                        0,
                        0,
                        hWnd,
                        reinterpret_cast<HMENU>(i),
                        hInstance,
                        nullptr
                    );

                    ::SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
                    ::SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

                    hwndLabel[i] = ::CreateWindowExW(
                        0L,
                        L"static",
                        wszColorLabel[i],
                        WS_CHILD | WS_VISIBLE | SS_CENTER,
                        0,
                        0,
                        0,
                        0,
                        hWnd,
                        reinterpret_cast<HMENU>(i + 3),
                        hInstance,
                        nullptr
                    );

                    hwndValue[i] = ::CreateWindowExW(
                        0L,
                        L"static",
                        L"0",
                        WS_CHILD | WS_VISIBLE | SS_CENTER,
                        0,
                        0,
                        0,
                        0,
                        hWnd,
                        reinterpret_cast<HMENU>(i + 6),
                        hInstance,
                        nullptr
                    );

                    oldScroll[i] = reinterpret_cast<WNDPROC>(::SetWindowLongPtrW(
                        hwndScroll[i],
                        GWLP_WNDPROC,
                        reinterpret_cast<
                            uintptr_t /* Petzold's example uses a LONG (4 bytes) here, that throws an access violation SEH in x64 */>(
                            ::ScrollHandler
                        )
                    ));
                    hBrush[i]    = ::CreateSolidBrush(crPrim[i]);
                }

                hStaticBrush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNHIGHLIGHT));
                cyChar       = HIWORD(::GetDialogBaseUnits());
                break;
            }

        case WM_SIZE :
            {
                cxClient = LOWORD(lParam);
                cyClient = HIWORD(lParam);

                SetRect(&rcColor, 0, 0, cxClient, cyClient);

                for (i = 0; i < 3; ++i) {
                    ::MoveWindow(hwndScroll[i], (2 * i + 1) * cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar, TRUE);
                    ::MoveWindow(hwndLabel[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
                    ::MoveWindow(hwndValue[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
                }

                ::SetFocus(hWnd);
                break;
            }

        case WM_SETFOCUS :
            {
                ::SetFocus(hwndScroll[idFocus]);
                break;
            }

        case WM_VSCROLL : // when the vertical scroll bars are moved
            {
                i = ::GetWindowLongPtrW(reinterpret_cast<HWND>(lParam), GWLP_ID);

                switch
                    LOWORD(wParam) {
                        case SB_PAGEDOWN      : color[i] += 20; [[fallthrough]];
                        case SB_LINEDOWN      : color[i] = std::min<INT32>(255, color[i] + 1); break;
                        case SB_PAGEUP        : color[i] -= 20; [[fallthrough]];
                        case SB_LINEUP        : color[i] = std::max(0, color[i] - 1); break;
                        case SB_TOP           : color[i] = 0; break;
                        case SB_BOTTOM        : color[i] = 255; break;
                        case SB_THUMBPOSITION : [[fallthrough]];
                        case SB_THUMBTRACK    : color[i] = HIWORD(wParam); break;
                        default               : break;
                    }

                ::SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
                ::StringCbPrintfExW(
                    wszBuffer,
                    sizeof(wszBuffer), // in bytes
                    reinterpret_cast<STRSAFE_LPWSTR*>(wszBuffer + 8),
                    nullptr,
                    STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL,
                    L"%03d",
                    color[i]
                );
                ::SetWindowTextW(hwndValue[i], wszBuffer);
                ::DeleteObject(reinterpret_cast<HBRUSH>(::SetClassLongPtrW(
                    hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<uintptr_t>(::CreateSolidBrush(RGB(color[0], color[1], color[2])))
                )));

                crTitleBar = RGB(color[0], color[1], color[2]);
                // DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crTitleBar, sizeof(COLORREF));
                // the line above colours the window borders, not the complete title bar.

                // colours the title bar
                ::DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &crTitleBar, sizeof(COLORREF));

                // update the hex colour code
                ::StringCbPrintfExW(
                    pswzHexColour,
                    sizeof(pswzHexColour) /* in bytes */,
                    reinterpret_cast<STRSAFE_LPWSTR*>(pswzHexColour + 8) /* end of buffer */,
                    nullptr,
                    STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE,
                    L"#%02X%02X%02X",
                    color[0],
                    color[1],
                    color[2]
                );
                ::SetWindowTextW(hwndTextBox, pswzHexColour);

                ::InvalidateRect(hWnd, &rcColor, TRUE);
                break;
            }

        case WM_COMMAND :
            {
                int wmId = LOWORD(wParam);
                // Parse the menu selections:
                switch (wmId) {
                    case IDM_EXIT : ::DestroyWindow(hWnd); break;
                    default       : break;
                }
                break;
            }

        case WM_DESTROY :
            {
                ::DeleteObject(reinterpret_cast<HBRUSH>(
                    ::SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<uintptr_t>(::GetStockObject(WHITE_BRUSH)))
                ));
                ::DeleteObject(hStaticBrush);

                for (i = 0; i < 3; ++i) ::DeleteObject(hBrush[i]);

                ::PostQuitMessage(0);
                break;
            }

        default : return ::DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}