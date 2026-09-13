#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <istream>
#include <map>
#include <string>
#include "token.hpp"
#include "type_utils.hpp"

namespace cmilan
{
  // Виды операций сравнения
  enum class cmp_operation
  {
    EQ, // Операция сравнения "="
    NE, // Операция сравнения "!="
    LT, // Операция сравнения "<"
    GT, // Операция сравнения ">"
    LE, // Операция сравнения "<="
    GE  // Операция сравнения ">="
  };

  // Виды арифметических операций
  enum class arithmetic_operation
  {
    PLUS,     // операция "+"
    MINUS,    // операция "-"
    MULTIPLY, // операция "*"
    DIVIDE    // операция "/"
  };

  // Лексический анализатор
  class scanner
  {
  public:
    // Конструктор. В качестве аргумента принимает имя файла и поток,
    // из которого будут читаться символы транслируемой программы.

    explicit scanner(std::istream& input) :
      ch_(),
      line_number_(1),
      int_value_(),
      token_(),
      cmp_value_(),
      arithmetic_value_(),
      input_(input)
    {

      next_char();
    }

    int get_line_number() const { return line_number_; }

    token get_token() const { return token_; }

    int get_int_value() const { return int_value_; }

    std::string get_string_value() const { return string_value_; }

    cmp_operation get_cmp_value() const { return cmp_value_; }

    arithmetic_operation get_arithmetic_value() const
    {
      return arithmetic_value_;
    }

    // Переход к следующей лексеме.
    // Текущая лексема записывается в token_ и изымается из потока.
    void next_token();

  private:
    char ch_;         // текущий символ
    int line_number_; // номер текущей строки кода
    int int_value_;   // значение текущего целого
    token token_;     // текущая лексема
    cmp_operation
      cmp_value_; // значение оператора сравнения (>, <, =, !=, >=, <=)
    arithmetic_operation arithmetic_value_; // значение знака (+,-,*,/)
    std::string string_value_;              // имя переменной

    std::istream& input_; // входной поток для чтения из файла.

    const inline static std::map< std::string, token > keywords_ = {
      { "begin", token::T_BEGIN },   { "end", token::T_END },
      { "repeat", token::T_REPEAT }, { "until", token::T_UNTIL },
      { "to", token::T_TO },         { "step", token::T_STEP },
      { "for", token::T_FOR },       { "if", token::T_IF },
      { "then", token::T_THEN },     { "else", token::T_ELSE },
      { "fi", token::T_FI },         { "while", token::T_WHILE },
      { "do", token::T_DO },         { "od", token::T_OD },
      { "write", token::T_WRITE },   { "read", token::T_READ },
    };
    // ассоциативный массив с лексемами и
    // соответствующими им зарезервированными словами в качестве индексов

    // Пропуск всех пробельные символы.
    // Если встречается символ перевода строки, номер текущей строки
    // (lineNumber) увеличивается на единицу.
    void skip_whitespace();

    void next_char(); // переходит к следующему символу
    // проверка переменной на первый символ (должен быть буквой латинского
    // алфавита)

    // проверка на остальные символы переменной (буква или цифра)
    bool is_identifier_body(char c)
    {
      return is_alpha(c) || is_digit(c);
    }
  };

}

#endif
