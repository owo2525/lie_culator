
#pragma once

#include <string>

class CalculatorLogic {
public:
    // 数字入力
    void inputNumber(int number);

    // 小数点入力
    void inputDecimal();

    // 演算子入力
    void setOperator(char op);

    // 計算実行
    void calculate();

    // 全消去
    void clear();

    // 表示用の計算式を取得
    std::wstring getExpressionDisplayValue() const;

    // 表示用の計算結果を取得
    std::wstring getResultDisplayValue() const;

    // 既存コードとの互換用
    std::wstring getDisplayValue() const;

private:
    // 入力中の計算式
    std::wstring expression = L"0";

    // 計算結果
    std::wstring resultDisplay;

    // 現在の数値を入力中か
    bool currentOperandStarted = false;

    // 計算完了後か
    bool afterResult = false;

    // エラー状態か
    bool hasError = false;

    // 正しい計算結果を取得
    double evaluateExpression() const;

    // 嘘の数値を生成
    double generateLie(double correctValue);

    // 数値を文字列に変換
    std::wstring formatNumber(double value);

    // 演算子判定
    bool isOperator(wchar_t value) const;

    // エラー処理
    void setError();
};
