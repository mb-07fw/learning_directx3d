#include <pch.hpp>
#include <App.hpp>

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
    CTM::CTMApp app;

    return app.Start();
}