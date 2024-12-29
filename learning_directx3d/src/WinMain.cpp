#include <pch.hpp>
#include <CustomWindow.hpp>
#include <CustomException.hpp>

/* Courtesy of ChiliTomatoNoodle's C++ 3D DirectX Tutorial (link -> https://www.youtube.com/playlist?list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD)
*   
*   Windows 101:
* 
*       1. Windows is about windows.
*       2. Windows is about messages.
*/

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,                               // Handle to the current instance of the application
    _In_opt_ HINSTANCE hPrevInstance,                       // Handle to the previous instance (unused, always NULL in modern Windows)
    _In_ LPSTR lpCmdLine,                                   // Command-line arguments as a single string
    _In_ int nCmdShow                                       // Flag that specifies how the window should be displayed
)
{
    try
    {
        CTM::CTMWindow window(
            640,
            480,
            "directx_test",
            "Direct3D Engine"
        );

        return window.MessageLoop();
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