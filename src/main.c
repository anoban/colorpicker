#include <colorpicker.h>
#define UNICODE
#define MAX_LOADSTRING    50LLU

#define MAINWINDOW_HEIGHT 260LLU
#define MAINWINDOW_WIDTH  490LLU

static WCHAR szTitle[MAX_LOADSTRING]       = { 0 }; // The title bar text
static WCHAR szWindowClass[MAX_LOADSTRING] = { 0 }; // the main window class name

// following are used in handlers.cpp
HINSTANCE    hApplicationInst              = NULL;  // the main window instance
INT32        iFocusedItemId                = 0;
WNDPROC      oldScroll[3]                  = { 0 }; // event handlers

// receives the handle to the current application instance, and registers it to WinGdi
static inline ATOM __stdcall RegisterMainWindowClass(HINSTANCE hInstance) {
    // creating a window class for the main application instance handle
    const WNDCLASSEXW wcxMainWindowClass = {
        .cbSize        = sizeof(WNDCLASSEX),
        .style         = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc   = WindowHandler,
        .cbClsExtra    = 0,
        .cbWndExtra    = 0,
        .hInstance     = hInstance,
        .hIcon         = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_COLORPICKER)),
        .hCursor       = LoadCursorW(nullptr, (LPWSTR) (IDC_ARROW)),
        //.hbrBackground = CreateSolidBrush(COLOR_WINDOW), // this will be the background colour of the main window and all it's child
        // window
        // unless those children are repainted with a different colour.
        .lpszMenuName  = MAKEINTRESOURCEW(IDC_COLORPICKER),
        .lpszClassName = szWindowClass,
        .hIconSm       = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_SMALL))
    };
    // register the main window class instance
    return RegisterClassExW(&wcxMainWindowClass);
}

// create and draw the main application window
static inline BOOL __stdcall DrawMainWindow(HINSTANCE hInstance, INT nCmdShow) {
    hApplicationInst = hInstance; // store the main window instance handle for global access

    HWND hWnd        = CreateWindowExW(
        WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE | WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME,
        szWindowClass,
        szTitle,
        WS_OVERLAPPED | WS_CLIPCHILDREN | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // we do not want the maximize button
        // specifying WS_MINIMIZEBOX requires WS_SYSMENU
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        MAINWINDOW_WIDTH,
        MAINWINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) return FALSE;

    return TRUE;
}

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    // initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COLORPICKER, szWindowClass, MAX_LOADSTRING);

    // register the main window, hInstance is the handle to the main application passed to the program by the windows runtime
    RegisterMainWindowClass(hInstance);

    // perform application initialization:
    if (!DrawMainWindow(hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_COLORPICKER));

    MSG    event;

    // the main event loop,
    while (GetMessageW(&event, nullptr, 0, 0)) {
        // changes in the main window and it's child windows will be received and stored in the variable `event`
        if (!TranslateAcceleratorW(event.hwnd, hAccelTable, &event)) {
            TranslateMessage(&event); // process the received message
            DispatchMessageW(&event); // dispatch it to the appropriate event handler, to trigger the desired response
        }
    }

    return (INT) (event.wParam);
}
