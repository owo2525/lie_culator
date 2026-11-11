
#include "CalculatorControls.h"

namespace {

    // ボタンID
    constexpr int ID_NUMBER_0 = 200;
    constexpr int ID_NUMBER_1 = 201;
    constexpr int ID_NUMBER_2 = 202;
    constexpr int ID_NUMBER_3 = 203;
    constexpr int ID_NUMBER_4 = 204;
    constexpr int ID_NUMBER_5 = 205;
    constexpr int ID_NUMBER_6 = 206;
    constexpr int ID_NUMBER_7 = 207;
    constexpr int ID_NUMBER_8 = 208;
    constexpr int ID_NUMBER_9 = 209;

    constexpr int ID_ADD = 300;
    constexpr int ID_SUBTRACT = 301;
    constexpr int ID_MULTIPLY = 302;
    constexpr int ID_DIVIDE = 303;

    constexpr int ID_CLEAR = 400;
    constexpr int ID_EQUAL = 500;
    constexpr int ID_DECIMAL = 600;

    // ラベル生成
    HWND createLabel(
        HWND parent,
        const wchar_t* text,
        int x,
        int y,
        int width,
        int height,
        HFONT font,
        DWORD style
    ) {

        HWND label = CreateWindowExW(
            0,
            L"STATIC",
            text,
            WS_CHILD | WS_VISIBLE | style,
            x,
            y,
            width,
            height,
            parent,
            nullptr,
            GetModuleHandleW(nullptr),
            nullptr
        );

        SendMessageW(
            label,
            WM_SETFONT,
            reinterpret_cast<WPARAM>(font),
            TRUE
        );

        return label;
    }

    // ボタン生成
    HWND createButton(
        HWND parent,
        const wchar_t* text,
        int id,
        int x,
        int y,
        int width,
        int height
    ) {

        return CreateWindowExW(
            0,
            L"BUTTON",
            text,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
            x,
            y,
            width,
            height,
            parent,
            reinterpret_cast<HMENU>(
                static_cast<INT_PTR>(id)
            ),
            GetModuleHandleW(nullptr),
            nullptr
        );
    }

}

// コントロール生成
void CalculatorControls::createControls(
    HWND parent,
    HFONT titleFont,
    HFONT subtitleFont,
    HFONT displayFont,
    HWND& displayBackground,
    HWND& expressionDisplay,
    HWND& resultDisplay
) {

    // タイトル
    createLabel(
        parent,
        L"lie_culator",
        20, 20, 320, 40,
        titleFont,
        SS_LEFT | SS_CENTERIMAGE
    );

    // サブタイトル
    createLabel(
        parent,
        L"Make your calculation a little fun!",
        20, 57, 320, 20,
        subtitleFont,
        SS_LEFT | SS_CENTERIMAGE
    );

    // ディスプレイ背景
    displayBackground = CreateWindowExW(
        0,
        L"STATIC",
        L"",
        WS_CHILD | WS_VISIBLE,
        20, 96, 320, 68,
        parent,
        nullptr,
        GetModuleHandleW(nullptr),
        nullptr
    );

    // 計算式表示
    // 親はメインウィンドウ
    expressionDisplay = createLabel(
        parent,
        L"0",
        30, 101, 300, 28,
        displayFont,
        SS_LEFT | SS_CENTERIMAGE
    );

    // 計算結果表示
    // 親はメインウィンドウ
    resultDisplay = createLabel(
        parent,
        L"",
        30, 129, 300, 30,
        displayFont,
        SS_RIGHT | SS_CENTERIMAGE
    );

    // ACボタン
    createButton(
        parent,
        L"AC",
        ID_CLEAR,
        20, 190, 320, 45
    );

    // ボタン情報
    struct ButtonInfo {
        const wchar_t* text;
        int id;
    };

    const ButtonInfo buttons[] = {

        { L"7", ID_NUMBER_7 },
        { L"8", ID_NUMBER_8 },
        { L"9", ID_NUMBER_9 },
        { L"÷", ID_DIVIDE },

        { L"4", ID_NUMBER_4 },
        { L"5", ID_NUMBER_5 },
        { L"6", ID_NUMBER_6 },
        { L"×", ID_MULTIPLY },

        { L"1", ID_NUMBER_1 },
        { L"2", ID_NUMBER_2 },
        { L"3", ID_NUMBER_3 },
        { L"-", ID_SUBTRACT },

        { L"0", ID_NUMBER_0 },
        { L".", ID_DECIMAL },
        { L"=", ID_EQUAL },
        { L"+", ID_ADD }

    };

    // ボタン配置
    constexpr int START_X = 20;
    constexpr int START_Y = 250;

    constexpr int BUTTON_WIDTH = 74;
    constexpr int BUTTON_HEIGHT = 60;
    constexpr int GAP = 8;

    constexpr int COLUMN_COUNT = 4;

    for (int i = 0; i < 16; i++) {

        int row = i / COLUMN_COUNT;
        int column = i % COLUMN_COUNT;

        int x = START_X + column * (BUTTON_WIDTH + GAP);
        int y = START_Y + row * (BUTTON_HEIGHT + GAP);

        createButton(
            parent,
            buttons[i].text,
            buttons[i].id,
            x,
            y,
            BUTTON_WIDTH,
            BUTTON_HEIGHT
        );
    }
}
