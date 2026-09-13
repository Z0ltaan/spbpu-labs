#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string_view>

namespace cmilan
{
  enum class token
  {
    T_EOF,        // Конец текстового потока
    T_ILLEGAL,    // Признак недопустимого символа
    T_IDENTIFIER, // Идентификатор
    T_FOR,        // Ключевое слово "for"
    T_TO,         // Ключевое слово "to"
    T_STEP,       // Ключевое слово "step"
    T_REPEAT,     // Ключевое слово "repeat"
    T_UNTIL,      // Ключевое слово "until"
    T_NUMBER,     // Целочисленный литерал
    T_BEGIN,      // Ключевое слово "begin"
    T_END,        // Ключевое слово "end"
    T_IF,         // Ключевое слово "if"
    T_THEN,       // Ключевое слово "then"
    T_ELSE,       // Ключевое слово "else"
    T_FI,         // Ключевое слово "fi"
    T_WHILE,      // Ключевое слово "while"
    T_DO,         // Ключевое слово "do"
    T_OD,         // Ключевое слово "od"
    T_WRITE,      // Ключевое слово "write"
    T_READ,       // Ключевое слово "read"
    T_ASSIGN,     // Оператор ":="
    T_ADDOP,      // Сводная лексема для "+" и "-" (операция типа сложения)
    T_MULOP,      // Сводная лексема для "*" и "/" (операция типа умножения)
    T_CMP,        // Сводная лексема для операторов отношения
    T_LPAREN,     // Открывающая скобка
    T_RPAREN,     // Закрывающая скобка
    T_SEMICOLON   // ";"
  };

  // Функция tokenToString возвращает описание лексемы.
  // Используется при печати сообщения об ошибке.
  constexpr std::string_view to_string(token t)
  {
    switch (t)
    {
      case token::T_EOF:
        return "end of file";
      case token::T_IDENTIFIER:
        return "identifier";
      case token::T_FOR:
        return "for";
      case token::T_TO:
        return "to";
      case token::T_STEP:
        return "step";
      case token::T_REPEAT:
        return "repeat";
      case token::T_UNTIL:
        return "until";
      case token::T_NUMBER:
        return "number";
      case token::T_BEGIN:
        return "begin";
      case token::T_END:
        return "end";
      case token::T_IF:
        return "if";
      case token::T_THEN:
        return "then";
      case token::T_ELSE:
        return "else";
      case token::T_FI:
        return "fi";
      case token::T_WHILE:
        return "while";
      case token::T_DO:
        return "do";
      case token::T_OD:
        return "od";
      case token::T_WRITE:
        return "write";
      case token::T_READ:
        return "read";
      case token::T_ASSIGN:
        return "':='";
      case token::T_ADDOP:
        return "'+' or '-'";
      case token::T_MULOP:
        return "'*' or '/'";
      case token::T_CMP:
        return "comparison operator";
      case token::T_LPAREN:
        return "'('";
      case token::T_RPAREN:
        return "')'";
      case token::T_SEMICOLON:
        return "';'";
      default:
        return "illegal token";
    }
  }
}

#endif
