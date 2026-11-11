
#include "CalculatorWindow.h"

#include "components/CalculatorControls.h"
#include "events/CalculatorEventHandler.h"
#include "styles/CalculatorStyle.h"

#include "../resource.h"

namespace {

    constexpr wchar_t WINDOW_CLASS_NAME[] =
        L"CalculatorWindowClass";

    constexpr wchar_t WINDOW_TITLE[] =
        L"Lie Calculator";

    constexpr int WINDOW_WIDTH = 380;
    constexpr int WINDOW_HEIGHT = 610;

    // 計算結果の文字色
    constexpr COLORREF RESULT_TEXT_COLOR =
        RGB(255, 220, 0);

}

// ==================================================
// ウィンドウ初期化
// ==================================================

void CalculatorWindow::initialize(
    HINSTANCE hInstance
) {

    // 背景ブラシ
    backgroundBrush = CreateSolidBrush(
        CalculatorStyle::getBackgroundColor()
    );

    // ディスプレイブラシ
    displayBrush = CreateSolidBrush(
        CalculatorStyle::getDisplayColor()
    );

    // ウィンドウクラス
    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CalculatorWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = backgroundBrush;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    // アプリアイコン
    wc.hIcon = LoadIconW(
        hInstance,
        MAKEINTRESOURCEW(IDI_APP_ICON)
    );

    // 小さいアイコン
    wc.hIconSm = LoadIconW(
        hInstance,
        MAKEINTRESOURCEW(IDI_APP_ICON)
    );

    RegisterClassExW(&wc);

    // メインウィンドウ生成
    hwnd = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    // ウィンドウ生成後にもアイコンを設定
    if (hwnd != nullptr) {

        HICON appIcon = LoadIconW(
            hInstance,
            MAKEINTRESOURCEW(IDI_APP_ICON)
        );

        SendMessageW(
            hwnd,
            WM_SETICON,
            ICON_BIG,
            reinterpret_cast<LPARAM>(appIcon)
        );

        SendMessageW(
            hwnd,
            WM_SETICON,
            ICON_SMALL,
            reinterpret_cast<LPARAM>(appIcon)
        );
    }
}

// ==================================================
// ウィンドウ表示
// ==================================================

void CalculatorWindow::show(
    int nCmdShow
) {

    if (hwnd == nullptr) {
        return;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
}

// ==================================================
// ウィンドウプロシージャ
// ==================================================

LRESULT CALLBACK CalculatorWindow::WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
) {

    CalculatorWindow* window =
        reinterpret_cast<CalculatorWindow*>(
            GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
            )
        );

    // インスタンス登録
    if (message == WM_NCCREATE) {

        CREATESTRUCTW* createStruct =
            reinterpret_cast<CREATESTRUCTW*>(lParam);

        CalculatorWindow* self =
            reinterpret_cast<CalculatorWindow*>(
                createStruct->lpCreateParams
            );

        SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(self)
        );

        self->hwnd = hwnd;

        window = self;
    }

    if (window == nullptr) {
        return DefWindowProcW(
            hwnd,
            message,
            wParam,
            lParam
        );
    }

    switch (message) {

    // ----------------------------------------------
    // ウィンドウ生成
    // ----------------------------------------------

    case WM_CREATE:
    {
        window->createFonts();

        CalculatorControls::createControls(
            hwnd,
            window->titleFont,
            window->subtitleFont,
            window->displayFont,
            window->displayBackground,
            window->expressionDisplay,
            window->resultDisplay
        );

        window->updateDisplay();

        return 0;
    }

    // ----------------------------------------------
    // ボタンクリック
    // ----------------------------------------------

    case WM_COMMAND:
    {
        CalculatorEventHandler::handleButtonClick(
            LOWORD(wParam),
            window->logic
        );

        window->updateDisplay();

        return 0;
    }

    // ----------------------------------------------
    // オーナードローボタン
    // ----------------------------------------------

    case WM_DRAWITEM:
    {
        DRAWITEMSTRUCT* drawInfo =
            reinterpret_cast<DRAWITEMSTRUCT*>(lParam);

        CalculatorStyle::drawButton(
            drawInfo,
            window->buttonFont
        );

        return TRUE;
    }

    // ----------------------------------------------
    // STATICコントロールの描画
    // ----------------------------------------------

    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = reinterpret_cast<HDC>(wParam);

        HWND control =
            reinterpret_cast<HWND>(lParam);

        // ディスプレイ背景
        if (control == window->displayBackground) {

            SetBkMode(hdc, OPAQUE);

            SetBkColor(
                hdc,
                CalculatorStyle::getDisplayColor()
            );

            return reinterpret_cast<LRESULT>(
                window->displayBrush
            );
        }

        // 計算式表示
        if (control == window->expressionDisplay) {

            SetTextColor(
                hdc,
                CalculatorStyle::getTextColor()
            );

            SetBkMode(hdc, OPAQUE);

            SetBkColor(
                hdc,
                CalculatorStyle::getDisplayColor()
            );

            return reinterpret_cast<LRESULT>(
                window->displayBrush
            );
        }

        // 計算結果表示
        if (control == window->resultDisplay) {

            SetTextColor(
                hdc,
                RESULT_TEXT_COLOR
            );

            SetBkMode(hdc, OPAQUE);

            SetBkColor(
                hdc,
                CalculatorStyle::getDisplayColor()
            );

            return reinterpret_cast<LRESULT>(
                window->displayBrush
            );
        }

        // タイトル・サブタイトルなど
        SetTextColor(
            hdc,
            CalculatorStyle::getTitleColor()
        );

        SetBkMode(hdc, TRANSPARENT);

        return reinterpret_cast<LRESULT>(
            window->backgroundBrush
        );
    }

    // ----------------------------------------------
    // 背景消去
    // ----------------------------------------------

    case WM_ERASEBKGND:
    {
        HDC hdc = reinterpret_cast<HDC>(wParam);

        RECT rect;

        GetClientRect(
            hwnd,
            &rect
        );

        FillRect(
            hdc,
            &rect,
            window->backgroundBrush
        );

        return TRUE;
    }

    // ----------------------------------------------
    // ウィンドウ破棄
    // ----------------------------------------------

    case WM_DESTROY:
    {
        window->releaseResources();

        PostQuitMessage(0);

        return 0;
    }

    // ----------------------------------------------
    // その他
    // ----------------------------------------------

    default:
        return DefWindowProcW(
            hwnd,
            message,
            wParam,
            lParam
        );
    }
}

// ==================================================
// フォント生成
// ==================================================

void CalculatorWindow::createFonts() {

    titleFont =
        CalculatorStyle::createTitleFont();

    subtitleFont =
        CalculatorStyle::createSubtitleFont();

    displayFont =
        CalculatorStyle::createDisplayFont();

    buttonFont =
        CalculatorStyle::createButtonFont();
}

// ==================================================
// リソース解放
// ==================================================

void CalculatorWindow::releaseResources() {

    if (titleFont != nullptr) {
        DeleteObject(titleFont);
        titleFont = nullptr;
    }

    if (subtitleFont != nullptr) {
        DeleteObject(subtitleFont);
        subtitleFont = nullptr;
    }

    if (displayFont != nullptr) {
        DeleteObject(displayFont);
        displayFont = nullptr;
    }

    if (buttonFont != nullptr) {
        DeleteObject(buttonFont);
        buttonFont = nullptr;
    }

    if (backgroundBrush != nullptr) {
        DeleteObject(backgroundBrush);
        backgroundBrush = nullptr;
    }

    if (displayBrush != nullptr) {
        DeleteObject(displayBrush);
        displayBrush = nullptr;
    }
}

// ==================================================
// ディスプレイ更新
// ==================================================

void CalculatorWindow::updateDisplay() {

    // 計算式
    if (expressionDisplay != nullptr) {

        SetWindowTextW(
            expressionDisplay,
            logic.getExpressionDisplayValue().c_str()
        );

        InvalidateRect(
            expressionDisplay,
            nullptr,
            TRUE
        );
    }

    // 計算結果
    if (resultDisplay != nullptr) {

        SetWindowTextW(
            resultDisplay,
            logic.getResultDisplayValue().c_str()
        );

        InvalidateRect(
            resultDisplay,
            nullptr,
            TRUE
        );
    }

    // メインウィンドウも再描画
    if (hwnd != nullptr) {
        UpdateWindow(expressionDisplay);
        UpdateWindow(resultDisplay);
    }
}
