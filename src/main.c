#include <colorpicker.h>

// STACK OVERFLOW HAPPENS WITH /LTCG !!! IN RELEASE MODE!
// FIX IT!

#define MAX_LOADSTRING    50LLU                                    // maximum length limit for strings to load in from colorpicker.rc

#define MAINWINDOW_HEIGHT 330LLU                                   // height of the application window
#define MAINWINDOW_WIDTH  610LLU                                   // width of the application window

static WCHAR                szTitle[MAX_LOADSTRING]       = { 0 }; // title bar text
static WCHAR                szWindowClass[MAX_LOADSTRING] = { 0 }; // the main window class name
HFONT                       hfLato                        = NULL;  // handle to font, our pick here is Lato
HDC                         hMonitorContext               = NULL;  // used to pick colours from the screen

// following globals are used in handlers.c
HINSTANCE                   hApplicationInst              = NULL; // the main window instance
HINSTANCE                   hPickerToolInstance           = NULL; // colour picker tool's window instance
INT32                       iFocusedItemId                = 0;

// receives the handle to the current application instance, and registers it to WinGdi
static inline ATOM CALLBACK RegisterMainWindowClass(_In_ const HINSTANCE hInstance) {
    // creating a window class for the main application instance handle
    const WNDCLASSEXW wcxMainWindowClass = { .cbSize        = sizeof(WNDCLASSEX),
                                             .style         = CS_HREDRAW | CS_VREDRAW,
                                             .lpfnWndProc   = WindowHandler,
                                             .cbClsExtra    = 0,
                                             .cbWndExtra    = 0,
                                             .hInstance     = hInstance,
                                             .hIcon         = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_COLORPICKER)),
                                             .hCursor       = LoadCursorW(NULL, (LPWSTR) (IDC_ARROW)),
                                             .hbrBackground = CreateSolidBrush(RGB(0, 0, 0)), // startup background colour will be black
                                             // this will be the background colour of the main window and all it's child windows
                                             // unless those children are repainted with a different colour.
                                             .lpszMenuName  = MAKEINTRESOURCEW(IDC_COLORPICKER),
                                             .lpszClassName = szWindowClass,
                                             .hIconSm       = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_SMALL)) };
    // register the main window class instance
    return RegisterClassExW(&wcxMainWindowClass); // returns 0 upon failure
}

// create and draw the main application window
static inline BOOL CALLBACK DrawMainWindow(_In_ const HINSTANCE hInstance, _In_ const int nCmdShow) {
    hApplicationInst   = hInstance; // store the main window instance handle for global access

    HWND hParentWindow = CreateWindowExW(
        // WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE | WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME,
        0,
        szWindowClass,
        szTitle,
        WS_OVERLAPPED | WS_CLIPCHILDREN | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // we do not want the maximize button
        // specifying WS_MINIMIZEBOX requires WS_SYSMENU
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        MAINWINDOW_WIDTH,
        MAINWINDOW_HEIGHT,
        NULL, // this is the parent window
        NULL,
        hInstance,
        NULL
    );
    // SetProcessDPIAware(); // escalates the window dpi, improves clarity and increases pixel density in the window
    ShowWindow(hParentWindow, nCmdShow);
    UpdateWindow(hParentWindow);
    if (!hParentWindow) return FALSE;

    return TRUE;
}

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    // initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COLORPICKER, szWindowClass, MAX_LOADSTRING);

    // register the main window, hInstance is the handle to the main application passed to the program by the windows runtime
    if (!RegisterMainWindowClass(hInstance)) {
        // TODO
    }

    static const INITCOMMONCONTROLSEX CommCtrlEx = { .dwSize = sizeof(INITCOMMONCONTROLSEX), .dwICC = ICC_WIN95_CLASSES };
    if (!InitCommonControlsEx(&CommCtrlEx)) {
        // TODO
    }

    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-logfonta
    // defining a logical font object to override the fugly default font
    static const LOGFONTW lfFontLato = { .lfHeight         = 0, // font mapper uses a default height value when it searches for a match.
                                         .lfWidth          = 0,
                                         .lfEscapement     = 0,
                                         .lfOrientation    = 0,
                                         .lfWeight         = FW_BOLD, // bold font weight
                                         .lfItalic         = FALSE,
                                         .lfUnderline      = FALSE,
                                         .lfStrikeOut      = FALSE,
                                         .lfCharSet        = ANSI_CHARSET, // we don't need any non-ASCII alphabets, so
                                         .lfOutPrecision   = OUT_TT_PRECIS,
                                         .lfClipPrecision  = CLIP_DEFAULT_PRECIS,
                                         .lfQuality        = ANTIALIASED_QUALITY,
                                         .lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS,
                                         .lfFaceName       = L"Lato" }; // assumes Lato is installed on the system

    hfLato                           = CreateFontIndirectW(&lfFontLato);

    SetProcessDPIAware(); // makes the process aware of the monitor's DPI, improves clarity and increases pixel density in the window

    // perform application initialization:
    if (!DrawMainWindow(hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_COLORPICKER));

    MSG    event;
    hMonitorContext = GetWindowDC(NULL);

    // the main event loop,
    while (GetMessageW(&event, NULL, 0, 0)) {
        // changes in the main window and it's child windows will be received and stored in the variable `event`
        if (!TranslateAcceleratorW(event.hwnd, hAccelTable, &event)) {
            TranslateMessage(&event); // process the received message
            DispatchMessageW(&event); // dispatch it to the appropriate event handler, to trigger the desired response
        }
    }

    return (INT) (event.wParam);
}
