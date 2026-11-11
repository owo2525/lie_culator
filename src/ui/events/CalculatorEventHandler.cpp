
#include "CalculatorEventHandler.h"

// ==================================================
// ボタンID
// ==================================================

namespace {

    constexpr int ID_NUMBER_BASE = 200;

    constexpr int ID_OPERATOR_ADD = 300;
    constexpr int ID_OPERATOR_SUBTRACT = 301;
    constexpr int ID_OPERATOR_MULTIPLY = 302;
    constexpr int ID_OPERATOR_DIVIDE = 303;

    constexpr int ID_CLEAR = 400;
    constexpr int ID_EQUAL = 500;
    constexpr int ID_DECIMAL = 600;

}

// ==================================================
// ボタンクリックイベント
// ==================================================

void CalculatorEventHandler::handleButtonClick(
    int id,
    CalculatorLogic& logic
) {

    // 数字入力
    if (id >= ID_NUMBER_BASE &&
        id <= ID_NUMBER_BASE + 9) {

        logic.inputNumber(id - ID_NUMBER_BASE);
    }

    // 小数点
    else if (id == ID_DECIMAL) {

        logic.inputDecimal();
    }

    // 加算
    else if (id == ID_OPERATOR_ADD) {

        logic.setOperator('+');
    }

    // 減算
    else if (id == ID_OPERATOR_SUBTRACT) {

        logic.setOperator('-');
    }

    // 乗算
    else if (id == ID_OPERATOR_MULTIPLY) {

        logic.setOperator('*');
    }

    // 除算
    else if (id == ID_OPERATOR_DIVIDE) {

        logic.setOperator('/');
    }

    // クリア
    else if (id == ID_CLEAR) {

        logic.clear();
    }

    // 計算
    else if (id == ID_EQUAL) {

        logic.calculate();
    }
}
