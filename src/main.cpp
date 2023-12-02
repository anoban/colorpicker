#include <colorpicker.hpp>
#include <framework.hpp>

constexpr std::size_t MAX_LOADSTRING { 100 };

// Global Variables:
HINSTANCE             hInst {};                         // current instance
WCHAR                 szTitle[MAX_LOADSTRING] {};       // The title bar text
WCHAR                 szWindowClass[MAX_LOADSTRING] {}; // the main window class name

INT32                 idFocus {};
WNDPROC               oldScroll[3] {};

// Message handler for about box.
INT_PTR CALLBACK      About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG : return (INT_PTR) TRUE;

        case WM_COMMAND :
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR) TRUE;
            }
            break;
    }
    return (INT_PTR) FALSE;
}

LRESULT CALLBACK ScrollProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    INT32 iId = GetWindowLongPtrW(hWnd, GWLP_ID);
    switch (message) {
        case WM_KEYDOWN :
            if (wParam == VK_TAB) SetFocus(GetDlgItem(GetParent(hWnd), (iId + (GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));
            break;
        case WM_SETFOCUS : idFocus = iId; break;
        default          : break;
    }
    return CallWindowProcW(oldScroll[iId], hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static COLORREF     crPrim[3] { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
    static HBRUSH       hBrush[3] {}, hStaticBrush {};
    static HWND         hwndScroll[3] {}, hwndLabel[3] {}, hwndValue[3] {}, hwndRect {};
    static INT32        color[3] {}, cyChar {};
    static RECT         rcColor {};
    static const WCHAR* wszColorLabel[] { L"Red", L"Green", L"Blue" };

    HINSTANCE           hInstance {};
    INT32               i {}, cxClient {}, cyClient {};
    WCHAR               wszBuffer[20] {};

    switch (message) {
        case WM_CREATE :
            {
                hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hWnd, GWLP_HINSTANCE));
                hwndRect  = CreateWindowExW(
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
                for (i = 0; i < 3; ++i) {
                    hwndScroll[i] = CreateWindowExW(
                        0L,
                        L"scrollbar",
                        nullptr,
                        WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT,
                        0,
                        0,
                        0,
                        0,
                        hWnd,
                        reinterpret_cast<HMENU>(i),
                        hInstance,
                        nullptr
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
                        reinterpret_cast<HMENU>(i + 3),
                        hInstance,
                        nullptr
                    );
                    hwndValue[i] = CreateWindowExW(
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
                    oldScroll[i] =
                        reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hwndScroll[i], GWLP_WNDPROC, reinterpret_cast<LONG>(ScrollProc)));
                    hBrush[i] = CreateSolidBrush(crPrim[i]);
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

        case WM_VSCROLL :
            {
                i = GetWindowLongPtrW(reinterpret_cast<HWND>(lParam), GWLP_ID);

                switch
                    LOWORD(wParam) {
                        case SB_PAGEDOWN      : color[i] += 15; [[fallthrough]];
                        case SB_LINEDOWN      : color[i] = std::min<INT32>(255, color[i] + 1); break;
                        case SB_PAGEUP        : color[i] -= 15; [[fallthrough]];
                        case SB_LINEUP        : color[i] = std::max(0, color[i] - 1); break;
                        case SB_TOP           : color[i] = 0; break;
                        case SB_BOTTOM        : color[i] = 255; break;
                        case SB_THUMBPOSITION : [[fallthrough]];
                        case SB_THUMBTRACK    : color[i] = HIWORD(wParam); break;
                        default               : break;
                    }
                SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
                wsprintfW(wszBuffer, L"%i", color[i]);
                SetWindowTextW(hwndValue[i], wszBuffer);
                DeleteObject(reinterpret_cast<HBRUSH>(
                    SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG>(CreateSolidBrush(RGB(color[0], color[1], color[2]))))
                ));
                InvalidateRect(hWnd, &rcColor, TRUE);
                break;
            }

        case WM_COMMAND :
            {
                int wmId = LOWORD(wParam);
                // Parse the menu selections:
                switch (wmId) {
                    case IDM_ABOUT : DialogBoxW(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
                    case IDM_EXIT  : DestroyWindow(hWnd); break;
                    default        : return DefWindowProcW(hWnd, message, wParam, lParam);
                }
                break;
            }
            

        case WM_DESTROY :
            {
                DeleteObject(reinterpret_cast<HBRUSH>(
                    SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG>(GetStockObject(WHITE_BRUSH)))
                ));
                DeleteObject(hStaticBrush);

                for (i = 0; i < 3; ++i) DeleteObject(hBrush[i]);

                PostQuitMessage(0);
                break;
            }

        default : return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex {};

    wcex.cbSize        = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLORPICKER));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(0);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_COLORPICKER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst     = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowExW(
        0L,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COLORPICKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    // HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_COLORPICKER));

    MSG    msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        // if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        // }
    }

    return (int) msg.wParam;
}