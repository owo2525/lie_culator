
#pragma once

#include "../../logic/CalculatorLogic.h"

class CalculatorEventHandler {

public:

    // ボタンクリックイベント
    static void handleButtonClick(
        int id,
        CalculatorLogic& logic
    );

};
