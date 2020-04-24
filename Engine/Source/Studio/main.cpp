/*
 Main Dynamik Studio Entry point file.

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		24/02/2020
 IDE:		MS Visual Studio Community 2019
*/

#include "studioafx.h"
#include "Platform/Windows/Window.h"

using namespace Platform::Windows;

int main() {
    MainWindow win;

    if (!win.Create(L"Learn to Program Windows", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    //ShowWindow(win.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;

	return 0;
}