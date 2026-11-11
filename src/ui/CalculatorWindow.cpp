
#include "CalculatorWindow.h"

#include "components/CalculatorControls.h"
#include "events/CalculatorEventHandler.h"
#include "styles/CalculatorStyle.h"

#include "../resource.h"

namespace {

    constexpr wchar_t WINDOW_CLASS_NAME[] =
        L"LieCulatorWindow";

    constexpr wchar_t WINDOW_TITLE[] =
        L"lie_culator";

    constexpr int WINDOW_WIDTH = 380;
    constexpr int WINDOW_HEIGHT = 570;

}

// ウィンドウ初期化
void CalculatorWindow::initialize(HINSTANCE hInstance) {

    backgroundBrush = CreateSolidBrush(
        CalculatorStyle::getBackgroundColor()
    );

    displayBrush = CreateSolidBrush(
        CalculatorStyle::getDisplayColor()
    );

    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = CalculatorWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = backgroundBrush;

    wc.hIcon = LoadIcon(
        hInstance,
        MAKEINTRESOURCE(IDI_APP_ICON)
    );

    wc.hIconSm = wc.hIcon;

    RegisterClassExW(&wc);

    hwnd = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        this
    );
}

// ウィンドウ表示
void CalculatorWindow::show(int nCmdShow) {

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

}

// ウィンドウプロシージャ
LRESULT CALLBACK CalculatorWindow::WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
) {

    CalculatorWindow* window =
        reinterpret_cast<CalculatorWindow*>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA)
        );

    if (message == WM_NCCREATE) {

        CREATESTRUCTW* createStruct =
            reinterpret_cast<CREATESTRUCTW*>(lParam);

        window =
            static_cast<CalculatorWindow*>(
                createStruct->lpCreateParams
            );

        SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(window)
        );

        window->hwnd = hwnd;
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

    case WM_CREATE:

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

    case WM_COMMAND:

        CalculatorEventHandler::handleButtonClick(
            LOWORD(wParam),
            window->logic
        );

        window->updateDisplay();

        return 0;

case WM_DRAWITEM:

    CalculatorStyle::drawButton(
        reinterpret_cast<DRAWITEMSTRUCT*>(lParam),
        window->buttonFont
    );

    return TRUE;

    case WM_CTLCOLORSTATIC: {

        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND control = reinterpret_cast<HWND>(lParam);

        // ディスプレイ背景
        if (control == window->displayBackground) {

            SetBkMode(hdc, OPAQUE);

            SetBkColor(
                hdc,
                CalculatorStyle::getDisplayColor()
            );

            SetTextColor(
                hdc,
                CalculatorStyle::getDisplayColor()
            );

            return reinterpret_cast<LRESULT>(
                window->displayBrush
            );
        }

        // 計算式表示
        if (control == window->expressionDisplay) {

            SetBkMode(hdc, TRANSPARENT);

            SetTextColor(
                hdc,
                RGB(255, 255, 255)
            );

            return reinterpret_cast<LRESULT>(
                GetStockObject(NULL_BRUSH)
            );
        }

        // 計算結果表示
        if (control == window->resultDisplay) {

            SetBkMode(hdc, TRANSPARENT);

            SetTextColor(
                hdc,
                RGB(243, 201, 105)
            );

            return reinterpret_cast<LRESULT>(
                GetStockObject(NULL_BRUSH)
            );
        }

        // タイトル・サブタイトル
        SetBkMode(hdc, TRANSPARENT);

        SetTextColor(
            hdc,
            RGB(37, 43, 54)
        );

        return reinterpret_cast<LRESULT>(
            window->backgroundBrush
        );
    }

    case WM_ERASEBKGND: {

        RECT rect;

        GetClientRect(hwnd, &rect);

        FillRect(
            reinterpret_cast<HDC>(wParam),
            &rect,
            window->backgroundBrush
        );

        return 1;
    }

    case WM_DESTROY:

        window->releaseResources();

        PostQuitMessage(0);

        return 0;

    }

    return DefWindowProcW(
        hwnd,
        message,
        wParam,
        lParam
    );
}

// フォント生成
void CalculatorWindow::createFonts() {

    titleFont = CreateFontW(
        28, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );

    subtitleFont = CreateFontW(
        13, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );

    displayFont = CreateFontW(
        20, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );

    buttonFont = CreateFontW(
        22, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );
}

// リソース解放
void CalculatorWindow::releaseResources() {

    if (titleFont) {
        DeleteObject(titleFont);
        titleFont = nullptr;
    }

    if (subtitleFont) {
        DeleteObject(subtitleFont);
        subtitleFont = nullptr;
    }

    if (displayFont) {
        DeleteObject(displayFont);
        displayFont = nullptr;
    }

    if (buttonFont) {
        DeleteObject(buttonFont);
        buttonFont = nullptr;
    }

    if (backgroundBrush) {
        DeleteObject(backgroundBrush);
        backgroundBrush = nullptr;
    }

    if (displayBrush) {
        DeleteObject(displayBrush);
        displayBrush = nullptr;
    }
}

// 表示更新
void CalculatorWindow::updateDisplay() {

    // 計算式表示
    if (expressionDisplay) {

        SetWindowTextW(
            expressionDisplay,
            logic.getExpressionDisplayValue().c_str()
        );
    }

    // 計算結果表示
    if (resultDisplay) {

        SetWindowTextW(
            resultDisplay,
            logic.getResultDisplayValue().c_str()
        );
    }
}
