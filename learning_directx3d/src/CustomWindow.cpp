#include <pch.hpp>
#include <CustomWindow.hpp>
#include <CustomException.hpp>

namespace CTM // (stands for custom)
{
    CTMWindow::CTMWindow(unsigned int width, unsigned int height, const char* wndClass, const char* wndTitle)
        : m_WndClass(wndClass), m_WndTitle(wndTitle), m_WndInstance(GetModuleHandle(nullptr))
    {
        // Register the window class.
        WNDCLASSEX wc = { 0 };

        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_WndInstance;
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszClassName = m_WndClass;
        wc.hIconSm = nullptr;

        RegisterClassEx(&wc);

        // Calculate window size to account for 640 x 480 client size.
        RECT wndRect;
        wndRect.left = 100;
        wndRect.right = width + wndRect.left;
        wndRect.top = 100;
        wndRect.bottom = height + wndRect.top;

        // If an error occurs, throw the exception with the error handle.
        if (AdjustWindowRect(&wndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
            THROW_CTM_WINDOW_EX_LAST;

        // Create the window.
        m_WndHandle = CreateWindowEx(
            0,
            m_WndClass,
            m_WndTitle,
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            wndRect.right - wndRect.left,
            wndRect.bottom - wndRect.top,
            nullptr,
            nullptr,
            m_WndInstance,
            this
        );

        if (m_WndHandle == nullptr)
            THROW_CTM_WINDOW_EX_LAST;

        // Show the window.
        ShowWindow(m_WndHandle, SW_SHOW);
    }

    CTMWindow::~CTMWindow() noexcept
	{
        DestroyWindow(m_WndHandle);
	}

    int CTMWindow::MessageLoop() noexcept
    {
        // "Message Pump"
        MSG msg;
        BOOL result;

        while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            // Translate raw virtual-key messages into character messages.
            TranslateMessage(&msg);

            // Forward the message to the current window procedure.
            DispatchMessage(&msg);
        }

        if (result == -1)
            return result;
        else
            return msg.wParam;
    }

    #pragma region Getters
	const char* CTMWindow::GetWndClass() noexcept
	{
		return m_WndClass;
	}

	const char* CTMWindow::GetWndTitle() noexcept
	{
		return m_WndTitle;
	}

    HINSTANCE CTMWindow::GetWndInstance() noexcept
    {
        return m_WndInstance;
    }
	
	HWND CTMWindow::GetWndHandle() noexcept
	{
		return m_WndHandle;
	}
    #pragma endregion

    #pragma region API Callbacks
    LRESULT CALLBACK CTMWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        // If we get a message before the WM_NCCREATE message, handle with default handler.
        if (msg != WM_NCCREATE)
            return DefWindowProc(hWnd, msg, wParam, lParam);

        // Else use create parameter passed in from CreateWindow() to store window class pointer.
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        CTMWindow* const pWnd = static_cast<CTMWindow*>(pCreate->lpCreateParams);

        // Set WinAPI-managed user data to store the pointer to Window.
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

        // Set message procedure to normal (non-setup) handler now that setup is finished.
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CTMWindow::HandleMsgThunk));

        // Forward message to window class handler.
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK CTMWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        // Retrieve pointer to Window.
        CTMWindow* const pWnd = reinterpret_cast<CTMWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        // Forward message to Window handler.
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    LRESULT CTMWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        switch (msg)
        {
        case WM_CLOSE:
            PostQuitMessage(69);
            return 69;
        case WM_KILLFOCUS:
            // clear keyboard.
            break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    #pragma endregion
}