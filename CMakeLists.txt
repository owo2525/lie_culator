cmake_minimum_required(VERSION 3.16)

project(calculator LANGUAGES CXX RC)

# ==================================================
# C++設定
# ==================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==================================================
# 実行ファイル
# ==================================================

add_executable(calculator WIN32

    # メイン
    src/main.cpp

    # リソース
    resources/app.rc

    # ロジック
    src/logic/CalculatorLogic.cpp

    # UI
    src/ui/CalculatorWindow.cpp

    # UI Components
    src/ui/components/CalculatorControls.cpp

    # UI Events
    src/ui/events/CalculatorEventHandler.cpp

    # UI Styles
    src/ui/styles/CalculatorStyle.cpp
)

# ==================================================
# インクルードディレクトリ
# ==================================================

target_include_directories(calculator PRIVATE
    src
)

# ==================================================
# Windows設定
# ==================================================

if(WIN32)

    target_compile_definitions(calculator PRIVATE
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )

endif()

# ==================================================
# MinGW 静的リンク設定
# ==================================================

if(MINGW)

    target_link_options(calculator PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )

endif()