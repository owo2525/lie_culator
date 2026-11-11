
#include <windows.h>
#include "ui/CalculatorWindow.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow
) {

    CalculatorWindow calculator;

    calculator.initialize(hInstance);
    calculator.show(nCmdShow);

    MSG msg = {};

    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.wParam);
}
