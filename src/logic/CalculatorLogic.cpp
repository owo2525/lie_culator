
#include "CalculatorLogic.h"

#include <cmath>
#include <cwchar>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>

namespace {

    // 乱数生成器
    std::mt19937& getRandomEngine() {
        static std::mt19937 engine(
            std::random_device{}()
        );

        return engine;
    }

    // 計算式解析クラス
    class ExpressionParser {
    public:
        explicit ExpressionParser(
            const std::wstring& expression
        )
            : expression(expression) {
        }

        double parse() {
            double value = parseExpression();

            if (position != expression.size()) {
                throw std::runtime_error("Invalid expression");
            }

            return value;
        }

    private:
        const std::wstring& expression;
        size_t position = 0;

        // 加算・減算
        double parseExpression() {
            double value = parseTerm();

            while (position < expression.size()) {
                wchar_t op = expression[position];

                if (op != L'+' && op != L'-') {
                    break;
                }

                position++;

                double right = parseTerm();

                if (op == L'+') {
                    value += right;
                } else {
                    value -= right;
                }
            }

            return value;
        }

        // 乗算・除算
        double parseTerm() {
            double value = parseFactor();

            while (position < expression.size()) {
                wchar_t op = expression[position];

                if (
                    op != L'*' &&
                    op != L'/' &&
                    op != L'×' &&
                    op != L'÷'
                ) {
                    break;
                }

                position++;

                double right = parseFactor();

                if (op == L'*' || op == L'×') {
                    value *= right;
                } else {
                    if (right == 0.0) {
                        throw std::runtime_error("Division by zero");
                    }

                    value /= right;
                }
            }

            return value;
        }

        // 数値・符号
        double parseFactor() {
            if (position >= expression.size()) {
                throw std::runtime_error("Invalid number");
            }

            if (expression[position] == L'+') {
                position++;
                return parseFactor();
            }

            if (expression[position] == L'-') {
                position++;
                return -parseFactor();
            }

            return parseNumber();
        }

        // 数値読み取り
        double parseNumber() {
            size_t start = position;

            bool hasDecimal = false;
            bool hasDigit = false;

            while (position < expression.size()) {
                wchar_t value = expression[position];

                if (value >= L'0' && value <= L'9') {
                    hasDigit = true;
                    position++;
                    continue;
                }

                if (value == L'.' && !hasDecimal) {
                    hasDecimal = true;
                    position++;
                    continue;
                }

                break;
            }

            if (!hasDigit) {
                throw std::runtime_error("Invalid number");
            }

            std::wstring number =
                expression.substr(start, position - start);

            return std::stod(number);
        }
    };

    // 現在入力中の数値の開始位置を取得
    size_t getCurrentNumberStart(
        const std::wstring& expression
    ) {
        size_t position =
            expression.find_last_of(L"+-*/×÷");

        if (position == std::wstring::npos) {
            return 0;
        }

        return position + 1;
    }
}

// 数字入力
void CalculatorLogic::inputNumber(int number) {

    if (number < 0 || number > 9) {
        return;
    }

    if (hasError || afterResult) {
        clear();
    }

    size_t start =
        getCurrentNumberStart(expression);

    std::wstring currentNumber =
        expression.substr(start);

    // 入力開始時、または現在の数値が0の場合
    if (!currentOperandStarted || currentNumber == L"0") {

        // 小数点入力後は0を置き換えない
        if (
            currentNumber == L"0" &&
            currentNumber.find(L'.') == std::wstring::npos
        ) {
            expression.replace(
                start,
                std::wstring::npos,
                std::to_wstring(number)
            );
        } else {
            expression += std::to_wstring(number);
        }

        currentOperandStarted = true;
        return;
    }

    expression += std::to_wstring(number);
}

// 小数点入力
void CalculatorLogic::inputDecimal() {

    if (hasError || afterResult) {
        clear();
    }

    size_t start =
        getCurrentNumberStart(expression);

    std::wstring currentNumber =
        expression.substr(start);

    if (currentNumber.find(L'.') != std::wstring::npos) {
        return;
    }

    if (!currentOperandStarted) {

        if (currentNumber.empty()) {
            expression += L"0.";
        } else {
            expression += L"0.";
        }

        currentOperandStarted = true;
        return;
    }

    expression += L'.';
}

// 演算子入力
void CalculatorLogic::setOperator(char op) {

    if (hasError) {
        return;
    }

    if (afterResult) {
        expression = resultDisplay;
        resultDisplay.clear();

        afterResult = false;
        currentOperandStarted = true;
    }

    wchar_t operation = static_cast<wchar_t>(op);

    if (operation == L'*') {
        operation = L'×';
    } else if (operation == L'/') {
        operation = L'÷';
    }

    if (
        operation != L'+' &&
        operation != L'-' &&
        operation != L'×' &&
        operation != L'÷'
    ) {
        return;
    }

    if (expression.empty()) {
        if (operation == L'-') {
            expression = L"-";
        }

        return;
    }

    wchar_t last = expression.back();

    // 演算子が連続した場合は置き換える
    if (isOperator(last)) {
        expression.back() = operation;
    } else {

        if (last == L'.') {
            expression += L'0';
        }

        expression += operation;
    }

    currentOperandStarted = false;
}

// 計算実行
void CalculatorLogic::calculate() {

    if (hasError || afterResult) {
        return;
    }

    if (
        expression.empty() ||
        isOperator(expression.back()) ||
        expression.back() == L'.'
    ) {
        return;
    }

    try {

        double correctValue = evaluateExpression();

        if (!std::isfinite(correctValue)) {
            setError();
            return;
        }

        double lieValue = generateLie(correctValue);

        resultDisplay = formatNumber(lieValue);

        afterResult = true;
        currentOperandStarted = false;

    } catch (...) {
        setError();
    }
}

// 正しい計算結果を取得
double CalculatorLogic::evaluateExpression() const {

    ExpressionParser parser(expression);

    return parser.parse();
}

// 嘘の数値を生成
double CalculatorLogic::generateLie(double correctValue) {

    std::mt19937& engine = getRandomEngine();

    std::uniform_int_distribution<int> patternDist(0, 4);

    std::uniform_real_distribution<double> randomDist(
        -1000000.0,
        1000000.0
    );

    std::uniform_real_distribution<double> multiplierDist(
        2.0,
        10.0
    );

    const std::wstring correctText =
        formatNumber(correctValue);

    for (int attempt = 0; attempt < 100; ++attempt) {

        double lieValue = correctValue;

        switch (patternDist(engine)) {

        case 0:
            lieValue = randomDist(engine);
            break;

        case 1:
            lieValue = -correctValue;
            break;

        case 2:
            lieValue = correctValue + randomDist(engine);
            break;

        case 3:
            lieValue = correctValue * multiplierDist(engine);
            break;

        case 4:
            lieValue = randomDist(engine);
            break;
        }

        if (!std::isfinite(lieValue)) {
            continue;
        }

        if (formatNumber(lieValue) != correctText) {
            return lieValue;
        }
    }

    if (correctValue == 0.0) {
        return 1.0;
    }

    return -correctValue;
}

// 数値を文字列に変換
std::wstring CalculatorLogic::formatNumber(double value) {

    if (value == 0.0) {
        value = 0.0;
    }

    std::wostringstream stream;

    stream << std::setprecision(15) << value;

    return stream.str();
}

// 演算子判定
bool CalculatorLogic::isOperator(wchar_t value) const {

    return (
        value == L'+' ||
        value == L'-' ||
        value == L'×' ||
        value == L'÷' ||
        value == L'*' ||
        value == L'/'
    );
}

// 全消去
void CalculatorLogic::clear() {

    expression = L"0";
    resultDisplay.clear();

    currentOperandStarted = false;
    afterResult = false;
    hasError = false;
}

// エラー処理
void CalculatorLogic::setError() {

    resultDisplay = L"Error";

    afterResult = true;
    currentOperandStarted = false;
    hasError = true;
}

// 計算式取得
std::wstring CalculatorLogic::getExpressionDisplayValue() const {

    if (afterResult && resultDisplay != L"Error") {
        return expression + L" =";
    }

    return expression;
}

// 計算結果取得
std::wstring CalculatorLogic::getResultDisplayValue() const {

    return resultDisplay;
}

// 既存コードとの互換用
std::wstring CalculatorLogic::getDisplayValue() const {

    if (!resultDisplay.empty()) {
        return resultDisplay;
    }

    return expression;
}
