#include <pch.hpp>
#include <CTMApp.hpp>

#ifdef _DEBUG

#define E

#else

#define F
#endif

namespace CTM // (stands for custom)
{
    CTMApp::CTMApp()
        : m_DeltaTime(), m_Window(640, 480, "directx_test", "Direct3D Engine"), 
          m_Timer()
    {

    }

    WPARAM CTMApp::Start()
    {
        if (!m_Window.IsWindowInitialized())
            exit(-1);

        MSG msg;
        BOOL result;

        MillisDuration frameStartTime;
        MillisDuration remainingTime;

        m_Timer.StartTimer();

        while ((result = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) >= 0)
        {
            // Break the loop if the message indicates a closure.
            if (msg.message == WM_QUIT)
                break;

            frameStartTime = m_Timer.ElapsedDuration();

            // Translate raw virtual-key messages into character messages.
            TranslateMessage(&msg);

            // Forward the message to the current window procedure.
            DispatchMessage(&msg);

            // Tick the app's frame.
            Tick();

            // Calculate delta and remaining time for the frame.
            m_DeltaTime = m_Timer.ElapsedDuration() - frameStartTime;

            // Calculate remaining time.
            remainingTime = SM_TARGET_FRAME_DURATION - m_DeltaTime;

            if (remainingTime.count() < 0)
                continue;
                
            // Sleep for the remaining frame duration.
            std::this_thread::sleep_for(remainingTime);
        }

        m_Timer.EndTimer();

        if (result == -1)
            return result;
        else
            return msg.wParam;
    }

    void CTMApp::Tick()
    {
        const std::unique_ptr<CTMGraphics>& graphics = m_Window.GetGraphics();
        const WindowSpace& windowSpace = m_Window.GetWindowSpace();
        const CTMMouse& mouse = m_Window.GetMouse();

        float mX = (float)mouse.GetCurrentX();
        float mY = (float)mouse.GetCurrentY();

        float normMx = mX / (windowSpace.width / 2) - 1.0f;
        float normMy = -mY / (windowSpace.height / 2) + 1.0f;

        float elapsed = (float)m_Timer.ElapsedSeconds();

        //SetWindowText(m_Window.GetWndHandle(), ("Norm | (" + std::to_string(normMx) + ", " + std::to_string(normMy)).c_str());
        //SetWindowText(m_Window.GetWndHandle(), std::to_string(elapsed).c_str());

        graphics->ClearBuffer(0, 0, 0);

        /*graphics->DrawTestTriangle(
            -elapsed,
            0.0f,
            0.0f
        );*/
        
        graphics->DrawTestTriangle(
            elapsed,
            normMx,
            normMy
        );

        graphics->EndFrame();

        /*if (m_Window.GetKeyboard().KeyIsPressed(VK_SPACE))
                    MessageBox(nullptr, "Somethan happon!", "Space Key Pressed!", MB_ICONEXCLAMATION);
        else if (m_Window.GetKeyboard().KeyIsPressed(VK_MENU))
            MessageBox(nullptr, "Somethan happon!", "Alt Key Pressed!", MB_ICONEXCLAMATION);*/
    }
}