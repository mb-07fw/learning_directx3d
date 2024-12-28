#include <pch.hpp>

/* Courtesy of ChiliTomatoNoodle's C++ 3D DirectX Tutorial (link -> https://www.youtube.com/playlist?list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD)
*   
*   Windows 101:
* 
*       1. Windows is about windows.
*       2. Windows is about messages.
* 
*   Note : I'm leaving documentation links for every WINAPI function so I don't forget what anything does.
*/

// (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(69);                                // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postquitmessage)
        break;
    case WM_KEYDOWN:
        if (wParam == 'F')
            SetWindowText(hWnd, L"Respects.");              // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowtextw)
        break;
    case WM_KEYUP:
        if (wParam == 'F')
            SetWindowText(hWnd, L"Test!!!");                // ↑
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);        // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-defwindowproca)
}

int CALLBACK WinMain(                                       // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain)
    HINSTANCE hInstance,                                    // Handle to the current instance of the application
    HINSTANCE hPrevInstance,                                // Handle to the previous instance (unused, always NULL in modern Windows)
    LPSTR lpCmdLine,                                        // Command-line arguments as a single string
    int nCmdShow                                            // Flag that specifies how the window should be displayed
)
{
    const WCHAR* pClassName = L"directx_test";

    // Register the window class
    WNDCLASSEX wc = { 0 };                                  // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa)

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;

    RegisterClassEx(&wc);                                   // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa)

    // Create the damn window
    HWND hWnd = CreateWindowEx(                             // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowa)
        0, pClassName,
        L"Test!!!",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 640, 480,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    // Show the damn window
    ShowWindow(hWnd, SW_SHOW);                              // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow)

    // "Message Pump"
    MSG msg;
    BOOL result;
    while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)  // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage)
    {
        TranslateMessage(&msg);                             // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage)
        DispatchMessage(&msg);                              // (Docs -> https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage)
    }

    if (result == -1)
        return -1;
    else
        return msg.wParam;                                  // (Docs -> https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-quit)

    return 0;
}