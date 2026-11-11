
#pragma once

#include <windows.h>

class CalculatorControls {

public:

    // コントロール生成
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
