
#include "CalculatorStyle.h"

namespace {

    // ==================================================
    // Accenture Color
    // ==================================================

    // メインカラー
    constexpr COLORREF ACCENT_PURPLE =
        RGB(161, 0, 255);       // #A100FF

    // ディープパープル
    constexpr COLORREF DEEP_PURPLE =
        RGB(117, 0, 192);       // #7500C0

    // ダークパープル
    constexpr COLORREF DARK_PURPLE =
        RGB(70, 0, 115);        // #460073

    // 背景色
    constexpr COLORREF BACKGROUND_COLOR =
        RGB(245, 242, 248);     // #F5F2F8

    // ディスプレイ背景色
    constexpr COLORREF DISPLAY_COLOR =
        RGB(35, 20, 48);

    // 白
    constexpr COLORREF WHITE =
        RGB(255, 255, 255);

    // 通常文字色
    constexpr COLORREF TEXT_COLOR =
        RGB(70, 0, 115);

    // ==================================================
    // Button ID
    // ==================================================

    constexpr int BUTTON_ID_ADD = 300;
    constexpr int BUTTON_ID_SUBTRACT = 301;
    constexpr int BUTTON_ID_MULTIPLY = 302;
    constexpr int BUTTON_ID_DIVIDE = 303;

    constexpr int BUTTON_ID_CLEAR = 400;
    constexpr int BUTTON_ID_EQUAL = 500;

    // ==================================================
    // Button Color
    // ==================================================

    COLORREF getButtonColor(int id) {

        // ACボタン
        if (id == BUTTON_ID_CLEAR) {
            return DARK_PURPLE;
        }

        // イコールボタン
        if (id == BUTTON_ID_EQUAL) {
            return DEEP_PURPLE;
        }

        // 演算子ボタン
        if (
            id == BUTTON_ID_ADD ||
            id == BUTTON_ID_SUBTRACT ||
            id == BUTTON_ID_MULTIPLY ||
            id == BUTTON_ID_DIVIDE
        ) {
            return ACCENT_PURPLE;
        }

        // 数字ボタン
        return WHITE;
    }

}

// ==================================================
// Color Getter
// ==================================================

// 背景色
COLORREF CalculatorStyle::getBackgroundColor() {
    return BACKGROUND_COLOR;
}

// ディスプレイ背景色
COLORREF CalculatorStyle::getDisplayColor() {
    return DISPLAY_COLOR;
}

// 通常文字色
COLORREF CalculatorStyle::getTextColor() {
    return ACCENT_PURPLE;
}

// タイトル文字色
COLORREF CalculatorStyle::getTitleColor() {
    return ACCENT_PURPLE;
}

// ==================================================
// Font
// ==================================================

// タイトルフォント
HFONT CalculatorStyle::createTitleFont() {

    return CreateFontW(
        26,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );

}

// サブタイトルフォント
HFONT CalculatorStyle::createSubtitleFont() {

    return CreateFontW(
        10,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );

}

// ディスプレイフォント
HFONT CalculatorStyle::createDisplayFont() {

    return CreateFontW(
        22,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );

}

// ボタンフォント
HFONT CalculatorStyle::createButtonFont() {

    return CreateFontW(
        16,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );

}

// ==================================================
// Button Drawing
// ==================================================

void CalculatorStyle::drawButton(
    DRAWITEMSTRUCT* drawItem,
    HFONT buttonFont
) {

    if (drawItem == nullptr) {
        return;
    }

    HDC hdc = drawItem->hDC;

    RECT rect = drawItem->rcItem;

    int buttonId = static_cast<int>(
        drawItem->CtlID
    );

    // ボタン色
    COLORREF buttonColor =
        getButtonColor(buttonId);

    // 押下状態
    bool isPressed =
        (drawItem->itemState & ODS_SELECTED) != 0;

    if (isPressed) {

        buttonColor = RGB(
            GetRValue(buttonColor) * 80 / 100,
            GetGValue(buttonColor) * 80 / 100,
            GetBValue(buttonColor) * 80 / 100
        );

    }

    // 背景ブラシ
    HBRUSH brush = CreateSolidBrush(
        buttonColor
    );

    // 枠線
    HPEN pen = CreatePen(
        PS_SOLID,
        1,
        buttonColor
    );

    HGDIOBJ oldBrush = SelectObject(
        hdc,
        brush
    );

    HGDIOBJ oldPen = SelectObject(
        hdc,
        pen
    );

    // ボタン背景
    RoundRect(
        hdc,
        rect.left,
        rect.top,
        rect.right,
        rect.bottom,
        12,
        12
    );

    // フォント
    HGDIOBJ oldFont = nullptr;

    if (buttonFont != nullptr) {

        oldFont = SelectObject(
            hdc,
            buttonFont
        );

    }

    // テキスト色
    if (
        buttonId >= 200 &&
        buttonId <= 209
    ) {

        // 数字ボタンはダークパープル
        SetTextColor(
            hdc,
            TEXT_COLOR
        );

    } else {

        // 演算子・AC・イコールは白
        SetTextColor(
            hdc,
            WHITE
        );

    }

    SetBkMode(
        hdc,
        TRANSPARENT
    );

    // ボタン文字列
    wchar_t text[32] = {};

    GetWindowTextW(
        drawItem->hwndItem,
        text,
        32
    );

    // 文字描画
    DrawTextW(
        hdc,
        text,
        -1,
        &rect,
        DT_CENTER |
        DT_VCENTER |
        DT_SINGLELINE
    );

    // フォーカス枠
    if (drawItem->itemState & ODS_FOCUS) {

        RECT focusRect = rect;

        InflateRect(
            &focusRect,
            -5,
            -5
        );

        DrawFocusRect(
            hdc,
            &focusRect
        );

    }

    // リソース復元
    if (oldFont != nullptr) {

        SelectObject(
            hdc,
            oldFont
        );

    }

    SelectObject(
        hdc,
        oldBrush
    );

    SelectObject(
        hdc,
        oldPen
    );

    // リソース解放
    DeleteObject(brush);
    DeleteObject(pen);
}
