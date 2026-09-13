#include "scanner.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include "type_utils.hpp"

void
cmilan::scanner::next_token()
{
  skip_whitespace();

  // Пропускаем комментарии
  // Если встречаем "/", то за ним должна идти "*". Если "*" не встречена,
  // считаем, что встретили операцию деления и лексему - операция типа
  // умножения. Дальше смотрим все символы, пока не находим звездочку или символ
  // конца файла. Если нашли * - проверяем на наличие "/" после нее. Если "/" не
  // найден - ищем следующую "*".
  while (ch_ == '/')
  {
    next_char();
    if (ch_ == '*')
    {
      next_char();
      bool inside = true;
      while (inside)
      {
        while (ch_ != '*' && !input_.eof())
        {
          next_char();
        }

        if (input_.eof())
        {
          token_ = token::T_EOF;
          return;
        }

        next_char();
        if (ch_ == '/')
        {
          inside = false;
          next_char();
        }
      }
    }
    else
    {
      token_ = token::T_MULOP;
      arithmetic_value_ = arithmetic_operation::DIVIDE;
      return;
    }

    skip_whitespace();
  }

  // Если встречен конец файла, считаем за лексему конца файла.
  if (input_.eof())
  {
    token_ = token::T_EOF;
    return;
  }
  // Если встретили цифру, то до тех пока дальше идут цифры - считаем как
  // продолжение числа. Запоминаем полученное целое, а за лексему считаем
  // целочисленный литерал

  if (cmilan::is_digit(ch_))
  {
    int value = 0;
    while (cmilan::is_digit(ch_))
    {
      value = value * 10 + (ch_ - '0'); // поразрядное считывание, преобразуем
                                        // символьное значение к числу.
      next_char();
    }
    token_ = token::T_NUMBER;
    int_value_ = value;
  }
  // Если же следующий символ - буква ЛА - тогда считываем до тех пор, пока
  // дальше буквы ЛА или цифры. Как только считали имя переменной, сравниваем ее
  // со списком зарезервированных слов. Если не совпадает ни с одним из них,
  // считаем, что получили переменную, имя которой запоминаем, а за текущую
  // лексему считаем лексему идентификатора. Если совпадает с каким-либо словом
  // из списка - считаем что получили лексему, соответствующую этому слову.
  else if (cmilan::is_alpha(ch_))
  {
    std::string buffer;
    while (is_identifier_body(ch_))
    {
      buffer += ch_;
      next_char();
    }

    std::transform(buffer.begin(),
                   buffer.end(),
                   buffer.begin(),
                   cmilan::to_lower< decltype(buffer)::value_type >);

    auto kwd = keywords_.find(buffer);
    if (kwd == keywords_.end())
    {
      token_ = token::T_IDENTIFIER;
      string_value_ = buffer;
    }
    else
    {
      token_ = kwd->second;
    }
  }
  // Символ не является буквой, цифрой, "/" или признаком конца файла
  else
  {
    switch (ch_)
    {
      // Признак лексемы открывающей скобки - встретили "("
      case '(':
        token_ = token::T_LPAREN;
        next_char();
        break;
      // Признак лексемы закрывающей скобки - встретили ")"
      case ')':
        token_ = token::T_RPAREN;
        next_char();
        break;
      // Признак лексемы ";" - встретили ";"
      case ';':
        token_ = token::T_SEMICOLON;
        next_char();
        break;
      // Если встречаем ":", то дальше смотрим наличие символа "=". Если
      // находим, то считаем что нашли лексему присваивания Иначе - лексема
      // ошибки.
      case ':':
        next_char();
        if (ch_ == '=')
        {
          token_ = token::T_ASSIGN;
          next_char();
        }
        else
        {
          token_ = token::T_ILLEGAL;
        }
        break;
      // Если встретили символ "<", то либо следующий символ "=", тогда лексема
      // нестрогого сравнения. Иначе - строгого.
      case '<':
        token_ = token::T_CMP;
        next_char();
        if (ch_ == '=')
        {
          cmp_value_ = cmp_operation::LE;
          next_char();
        }
        else
        {
          cmp_value_ = cmp_operation::LT;
        }
        break;
      // Аналогично предыдущему случаю
      case '>':
        token_ = token::T_CMP;
        next_char();
        if (ch_ == '=')
        {
          cmp_value_ = cmp_operation::GE;
          next_char();
        }
        else
        {
          cmp_value_ = cmp_operation::GT;
        }
        break;
      // Если встретим "!", то дальше должно быть "=", тогда считаем, что
      // получили лексему сравнения и знак "!=" иначе считаем, что у нас лексема
      // ошибки
      case '!':
        next_char();
        if (ch_ == '=')
        {
          next_char();
          token_ = token::T_CMP;
          cmp_value_ = cmp_operation::NE;
        }
        else
        {
          token_ = token::T_ILLEGAL;
        }
        break;
      // Если встретим "=" - лексема сравнения и знак "="
      case '=':
        token_ = token::T_CMP;
        cmp_value_ = cmp_operation::EQ;
        next_char();
        break;
      // Знаки операций. Для "+"/"-" получим лексему операции типа сложнения, и
      // соответствующую операцию. для "*" - лексему операции типа умножения
      case '+':
        token_ = token::T_ADDOP;
        arithmetic_value_ = arithmetic_operation::PLUS;
        next_char();
        break;

      case '-':
        token_ = token::T_ADDOP;
        arithmetic_value_ = arithmetic_operation::MINUS;
        next_char();
        break;

      case '*':
        token_ = token::T_MULOP;
        arithmetic_value_ = arithmetic_operation::MULTIPLY;
        next_char();
        break;
      default:
        token_ = token::T_ILLEGAL;
        next_char();
        break;
    }
  }
}

void
cmilan::scanner::skip_whitespace()
{
  while (cmilan::is_space(ch_))
  {
    if (ch_ == '\n')
    {
      ++line_number_;
    }

    next_char();
  }
}

void
cmilan::scanner::next_char()
{
  ch_ = input_.get();
}
