#ifndef CMILAN_PARSER_HPP
#define CMILAN_PARSER_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "codegen.hpp"
#include "scanner.hpp"
#include "token.hpp"

namespace cmilan
{
  /* Синтаксический анализатор.
   *
   * Задачи:
   * - проверка корректности программы,
   * - генерация кода для виртуальной машины в процессе анализа,
   * - простейшее восстановление после ошибок.
   *
   * Синтаксический анализатор языка Милан.
   *
   * Парсер с помощью переданного ему при инициализации лексического анализатора
   * читает по одной лексеме и на основе грамматики Милана генерирует код для
   * стековой виртуальной машины. Синтаксический анализ выполняется методом
   * рекурсивного спуска.
   *
   * При обнаружении ошибки парсер печатает сообщение и продолжает анализ со
   * следующего оператора, чтобы в процессе разбора найти как можно больше
   * ошибок. Поскольку стратегия восстановления после ошибки очень проста,
   * возможна печать сообщений о несуществующих ("наведенных") ошибках или
   * пропуск некоторых ошибок без печати сообщений. Если в процессе разбора была
   * найдена хотя бы одна ошибка, код для виртуальной машины не печатается.*/

  class parser
  {
  public:
    // Конструктор
    //    const string& fileName - имя файла с программой для анализа
    //
    // Конструктор создает экземпляры лексического анализатора и генератора.

    explicit parser(std::istream& input) :
      error_(false),
      recovered_(true),
      last_var_number_(0),
      output_(std::cout),
      scanner_(std::make_unique< decltype(scanner_)::element_type >(input)),
      codegen_(std::make_unique< decltype(codegen_)::element_type >()),
      variables_()
    {
      next();
    }

    decltype(auto) parse()
    {
      must_be(token::T_BEGIN);
      read_statement_list();
      must_be(token::T_END);
      codegen_->emit(command::vm_instruction::STOP);

      return codegen_->get_command_buffer();
    }

    bool error() const noexcept { return error_; }

  private:
    using var_table = std::map< std::string, int >;

    bool error_;     // флаг ошибки. Используется чтобы определить, выводим ли
                     // список команд после разбора или нет
    bool recovered_; // не используется
    int last_var_number_; // номер последней записанной переменной

    std::ostream& output_; // выходной поток (в данном случае используем cout)
    std::unique_ptr< scanner >
      scanner_; // лексический анализатор для конструктора
    std::unique_ptr< code_generator >
      codegen_;           // указатель на виртуальную машину
    var_table variables_; // массив переменных, найденных в программе

    // описание блоков.
    void program();             // Разбор программы. BEGIN statementList END
    void read_statement_list(); // Разбор списка операторов.
    void read_statement();      // разбор оператора.
    void read_expression();     // разбор арифметического выражения.
    void read_term();           // разбор слагаемого.
    void read_factor();         // разбор множителя.
    void read_relation();       // разбор условия.
    void read_cfor_loop();
    void read_for_loop();
    void read_repeat_until(); // Разбор цикла repeat-until

    // Сравнение текущей лексемы с образцом. Текущая позиция в потоке лексем не
    // изменяется.
    bool see(token t) { return scanner_->get_token() == t; }

    // Проверка совпадения текущей лексемы с образцом. Если лексема и образец
    // совпадают, лексема изымается из потока.

    bool match(token t)
    {
      if (scanner_->get_token() == t)
      {
        scanner_->next_token();
        return true;
      }
      else
      {
        return false;
      }
    }

    // Переход к следующей лексеме.

    void next() { scanner_->next_token(); }

    // Обработчик ошибок.
    void report_error(const std::string& message)
    {
      std::cerr << "Line " << scanner_->get_line_number() << ": " << message
                << std::endl;
      error_ = true;
    }

    void must_be(token t); // проверяем, совпадает ли данная лексема с образцом.
                           // Если да, то лексема изымается из потока.
    // Иначе создаем сообщение об ошибке и пробуем восстановиться
    void recover(token t); // восстановление после ошибки: идем по коду до тех
                           // пор,
    // пока не встретим эту лексему или лексему конца файла.
    int find_or_add_variable(
      const std::string&); // функция пробегает по variables_.
    // Если находит нужную переменную - возвращает ее номер, иначе добавляет ее
    // в массив, увеличивает lastVar и возвращает его.
  };

}

#endif
