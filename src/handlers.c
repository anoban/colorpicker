#include <colorpicker.h>

extern HINSTANCE hInst;
extern INT32     idFocus;
extern WNDPROC   oldScroll[3];

LRESULT CALLBACK ScrollHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    INT64 iId = GetWindowLongPtrW(hWnd, GWLP_ID);
    switch (message) {
        case WM_KEYDOWN :
            if (wParam == VK_TAB) SetFocus(GetDlgItem(GetParent(hWnd), (iId + (GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));
            break;
        case WM_SETFOCUS : idFocus = iId; break;
        default          : break;
    }
    return CallWindowProcW(oldScroll[iId], hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF crPrim[3]  = { RGB(32, 32, 32), RGB(32, 32, 32), RGB(32, 32, 32) };
    static COLORREF crTitleBar = { RGB(32, 32, 32) };
    static HBRUSH   hBrush[3] = { 0 }, hStaticBrush = { 0 };
    static HWND     hwndScroll[3] = { 0 } /* scroll bars */, hwndLabel[3] = { 0 } /* labels */, hwndValue[3] = { 0 } /* label texts */,
                hwndRect            = { 0 };
    static HWND         hwndTextBox = { 0 };
    static INT32        color[3] = { 0 }, cyChar = 0;
    static RECT         rcColor         = { 0 };
    static const WCHAR* wszColorLabel[] = { L"Red", L"Green", L"Blue" };
    static WCHAR   pswzHexColour[8] = { 0 }; // needs to be in the static memory since these are used even wehn the callback isn't invoked
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-logfonta
    static LOGFONT lfFontAttrs      = { 0 };
    static const HFONT hFont        = CreateFontIndirectW(&lfFontAttrs);
    HINSTANCE          hInstance    = { 0 };
    INT64              i = 0, cxClient = 0, cyClient = 0;
    static WCHAR       wszBuffer[8] = { 0 }; // needs to be in the static memory since these are used even wehn the callback isn't invoked
    static const BOOL  bUseDarkMode = TRUE;  // make the title bar

    switch (message) {
        case WM_CREATE :
            {
                hInstance = (HINSTANCE) (GetWindowLongPtrW(hWnd, GWLP_HINSTANCE));
                SetMenu(hWnd, NULL);         // hide the menu bar

                // https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
                DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &bUseDarkMode, sizeof(BOOL));
                DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crTitleBar, sizeof(COLORREF));

                hwndRect = CreateWindowExW(
                    0L, L"static", NULL, WS_CHILD | WS_VISIBLE | SS_WHITERECT, 0, 0, 0, 0, hWnd, (HMENU) (9), hInstance, NULL
                );

                hwndTextBox = CreateWindowExW(
                    WS_EX_CLIENTEDGE,
                    L"edit",
                    NULL,
                    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_OVERLAPPED,
                    300,
                    10,
                    100,
                    22,
                    hWnd,
                    (HMENU) (10),
                    hInstance,
                    NULL
                );

                for (i = 0; i < 3; ++i) {
                    hwndScroll[i] = CreateWindowExW(
                        0L,
                        L"scrollbar",
                        NULL,
                        WS_CHILD | WS_VISIBLE | SBS_VERT | WS_TABSTOP,
                        0,
                        0,
                        0,
                        0,
                        hWnd,
                        (HMENU) (i),
                        hInstance,
                        NULL
                    );

                    SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
                    SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

                    hwndLabel[i] = CreateWindowExW(
                        0L,
                        L"static",
                        wszColorLabel[i],
                        WS_CHILD | WS_VISIBLE | SS_CENTER,
                        0,
                        0,
                        0,
                        0,
                        hWnd,
                        (HMENU) (i + 3),
                        hInstance,
                        NULL
                    );

                    hwndValue[i] = CreateWindowExW(
                        0L, L"static", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, hWnd, (HMENU) (i + 6), hInstance, NULL
                    );

                    oldScroll[i] = (WNDPROC) (SetWindowLongPtrW(
                        hwndScroll[i],
                        GWLP_WNDPROC,
                        (uintptr_t /* Petzold's example uses a LONG (4 bytes) here, that throws an access violation SEH in x64 */)
                            ScrollHandler
                    ));
                    hBrush[i]    = CreateSolidBrush(crPrim[i]);
                }

                hStaticBrush = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
                cyChar       = HIWORD(GetDialogBaseUnits());
                break;
            }

        case WM_SIZE :
            {
                cxClient = LOWORD(lParam);
                cyClient = HIWORD(lParam);

                SetRect(&rcColor, 0, 0, cxClient, cyClient);

                for (i = 0; i < 3; ++i) {
                    MoveWindow(hwndScroll[i], (2 * i + 1) * cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar, TRUE);
                    MoveWindow(hwndLabel[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
                    MoveWindow(hwndValue[i], (4 * i + 1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
                }

                SetFocus(hWnd);
                break;
            }

        case WM_SETFOCUS :
            {
                SetFocus(hwndScroll[idFocus]);
                break;
            }

        case WM_VSCROLL : // when the vertical scroll bars are moved
            {
                i = GetWindowLongPtrW((HWND) (lParam), GWLP_ID);

                switch
                    LOWORD(wParam) {
                        case SB_PAGEDOWN      : color[i] += 20; [[fallthrough]];
                        case SB_LINEDOWN      : color[i] = stdmin<INT32>(255, color[i] + 1); break;
                        case SB_PAGEUP        : color[i] -= 20; [[fallthrough]];
                        case SB_LINEUP        : color[i] = stdmax(0, color[i] - 1); break;
                        case SB_TOP           : color[i] = 0; break;
                        case SB_BOTTOM        : color[i] = 255; break;
                        case SB_THUMBPOSITION : [[fallthrough]];
                        case SB_THUMBTRACK    : color[i] = HIWORD(wParam); break;
                        default               : break;
                    }

                SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
                StringCbPrintfExW(
                    wszBuffer,
                    sizeof(wszBuffer), // in bytes
                    (STRSAFE_LPWSTR*) (wszBuffer + 8),
                    NULL,
                    STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL,
                    L"%03d",
                    color[i]
                );
                SetWindowTextW(hwndValue[i], wszBuffer);
                DeleteObject(
                    (HBRUSH) (SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (uintptr_t) (CreateSolidBrush(RGB(color[0], color[1], color[2])))))
                );

                crTitleBar = RGB(color[0], color[1], color[2]);
                // DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &crTitleBar, sizeof(COLORREF));
                // the line above colours the window borders, not the complete title bar.

                // colours the title bar
                DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &crTitleBar, sizeof(COLORREF));

                // update the hex colour code
                StringCbPrintfExW(
                    pswzHexColour,
                    sizeof(pswzHexColour) /* in bytes */,
                    (STRSAFE_LPWSTR*) (pswzHexColour + 8) /* end of buffer */,
                    NULL,
                    STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE,
                    L"#%02X%02X%02X",
                    color[0],
                    color[1],
                    color[2]
                );
                SetWindowTextW(hwndTextBox, pswzHexColour);

                InvalidateRect(hWnd, &rcColor, TRUE);
                break;
            }

        case WM_COMMAND :
            {
                int wmId = LOWORD(wParam);
                // Parse the menu selections:
                switch (wmId) {
                    case IDM_EXIT : DestroyWindow(hWnd); break;
                    default       : break;
                }
                break;
            }

        case WM_DESTROY :
            {
                DeleteObject((HBRUSH) (SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (uintptr_t) (GetStockObject(WHITE_BRUSH)))));
                DeleteObject(hStaticBrush);

                for (i = 0; i < 3; ++i) DeleteObject(hBrush[i]);

                PostQuitMessage(0);
                break;
            }

        default : return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}