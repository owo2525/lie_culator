
#include "CalculatorStyle.h"

namespace {

    // カラー定義
    constexpr COLORREF CALCULATOR_BACKGROUND =
        RGB(247, 243, 234);

    constexpr COLORREF CALCULATOR_DISPLAY =
        RGB(37, 43, 54);

    constexpr COLORREF CALCULATOR_NUMBER =
        RGB(255, 255, 255);

    constexpr COLORREF CALCULATOR_OPERATOR =
        RGB(90, 160, 210);

    constexpr COLORREF CALCULATOR_CLEAR =
        RGB(100, 180, 130);

    constexpr COLORREF CALCULATOR_EQUAL =
        RGB(150, 120, 200);

    constexpr COLORREF CALCULATOR_TEXT =
        RGB(45, 45, 55);

    constexpr COLORREF CALCULATOR_WHITE =
        RGB(255, 255, 255);

    // ボタンID
    constexpr int BUTTON_ID_ADD = 300;
    constexpr int BUTTON_ID_SUBTRACT = 301;
    constexpr int BUTTON_ID_MULTIPLY = 302;
    constexpr int BUTTON_ID_DIVIDE = 303;
    constexpr int BUTTON_ID_CLEAR = 400;
    constexpr int BUTTON_ID_EQUAL = 500;

    // ボタン色を取得
    COLORREF getButtonColor(int id) {

        if (id == BUTTON_ID_CLEAR) {
            return CALCULATOR_CLEAR;
        }

        if (id == BUTTON_ID_EQUAL) {
            return CALCULATOR_EQUAL;
        }

        if (
            id == BUTTON_ID_ADD ||
            id == BUTTON_ID_SUBTRACT ||
            id == BUTTON_ID_MULTIPLY ||
            id == BUTTON_ID_DIVIDE
        ) {
            return CALCULATOR_OPERATOR;
        }

        return CALCULATOR_NUMBER;
    }

}

// 背景色
COLORREF CalculatorStyle::getBackgroundColor() {
    return CALCULATOR_BACKGROUND;
}

// ディスプレイ背景色
COLORREF CalculatorStyle::getDisplayColor() {
    return CALCULATOR_DISPLAY;
}

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

// ボタン描画
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
    COLORREF buttonColor = getButtonColor(buttonId);

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
    HBRUSH brush = CreateSolidBrush(buttonColor);

    // 枠線
    HPEN pen = CreatePen(
        PS_SOLID,
        1,
        buttonColor
    );

    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    HGDIOBJ oldPen = SelectObject(hdc, pen);

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
        oldFont = SelectObject(hdc, buttonFont);
    }

    // テキスト色
    if (
        buttonId >= 200 &&
        buttonId <= 209
    ) {
        SetTextColor(hdc, CALCULATOR_TEXT);
    } else {
        SetTextColor(hdc, CALCULATOR_WHITE);
    }

    SetBkMode(hdc, TRANSPARENT);

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
        SelectObject(hdc, oldFont);
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    DeleteObject(brush);
    DeleteObject(pen);
}
