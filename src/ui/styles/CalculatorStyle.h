
#pragma once

#include <windows.h>

class CalculatorStyle {

public:

    // 背景色
    static COLORREF getBackgroundColor();

    // ディスプレイ背景色
    static COLORREF getDisplayColor();

    // 文字色
    static COLORREF getTextColor();

    // タイトル文字色
    static COLORREF getTitleColor();

    // フォント生成
    static HFONT createTitleFont();
    static HFONT createSubtitleFont();
    static HFONT createDisplayFont();
    static HFONT createButtonFont();

    // ボタン描画
    static void drawButton(
        DRAWITEMSTRUCT* drawItem,
        HFONT buttonFont
    );

};
