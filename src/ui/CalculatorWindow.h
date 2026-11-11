
#pragma once

#include <windows.h>
#include <string>

#include "../logic/CalculatorLogic.h"

class CalculatorWindow {

public:

    // ウィンドウ初期化
    void initialize(HINSTANCE hInstance);

    // ウィンドウ表示
    void show(int nCmdShow);

private:

    // メインウィンドウ
    HWND hwnd = nullptr;

    // ディスプレイ
    HWND displayBackground = nullptr;
    HWND expressionDisplay = nullptr;
    HWND resultDisplay = nullptr;

    // 計算ロジック
    CalculatorLogic logic;

    // フォント
    HFONT titleFont = nullptr;
    HFONT subtitleFont = nullptr;
    HFONT displayFont = nullptr;
    HFONT buttonFont = nullptr;

    // ブラシ
    HBRUSH backgroundBrush = nullptr;
    HBRUSH displayBrush = nullptr;

    // ウィンドウプロシージャ
    static LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );

    // フォント生成
    void createFonts();

    // リソース解放
    void releaseResources();

    // 表示更新
    void updateDisplay();

};
