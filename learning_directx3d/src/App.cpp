#include <pch.hpp>
#include <App.hpp>

namespace CTM // (stands for custom)
{
    CTMApp::CTMApp()
        : m_Window(640, 480, "directx_test", "Direct3D Engine")
    {

    }

    int CTMApp::Start()
    {
        try
        {
            // "Message Pump"
            MSG msg;
            BOOL result;

            m_Timer.StartTimer();

            while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
            {
                // Translate raw virtual-key messages into character messages.
                TranslateMessage(&msg);

                // Forward the message to the current window procedure.
                DispatchMessage(&msg);

                Tick();

                /*if (m_Keyboard.KeyIsPressed(VK_SPACE))
                    MessageBox(nullptr, "Somethan happon!", "Space Key Pressed!", MB_ICONEXCLAMATION);
                else if (m_Keyboard.KeyIsPressed(VK_MENU))
                    MessageBox(nullptr, "Somethan happon!", "Alt Key Pressed!", MB_ICONEXCLAMATION);*/
            }

            m_Timer.EndTimer();

            if (result == -1)
                return result;
            else
                return msg.wParam;
        }
        catch (const CTM::CTMException& ex)
        {
            MessageBox(nullptr, ex.what(), ex.GetType(), MB_OK | MB_ICONERROR);
        }
        catch (const std::exception& ex)
        {
            MessageBox(nullptr, ex.what(), "Standard exception.", MB_OK | MB_ICONERROR);
        }
        catch (...)
        {
            MessageBox(nullptr, "No details available.", "Unknown Exception", MB_OK | MB_ICONERROR);
        }
    }

    void CTMApp::Tick()
    {
        SetWindowText(m_Window.GetWndHandle(), ("Elapsed : " + std::to_string(m_Timer.ElapsedSeconds())).c_str());
    }
}