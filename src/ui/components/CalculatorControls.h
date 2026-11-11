
#pragma once

#include <windows.h>

class CalculatorControls {

public:

    static void createControls(
        HWND parent,
        HFONT titleFont,
        HFONT subtitleFont,
        HFONT displayFont,
        HWND& displayBackground,
        HWND& expressionDisplay,
        HWND& resultDisplay
    );

};
