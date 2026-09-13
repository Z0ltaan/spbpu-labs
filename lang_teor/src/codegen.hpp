#ifndef CMILAN_CODEGEN_HPP
#define CMILAN_CODEGEN_HPP

#include <iostream>
#include <ostream>
#include <vector>

namespace cmilan
{
  // Класс Command представляет машинные инструкции.
  class command
  {
  public:
    enum class vm_instruction
    {
      NOP,  // отсутствие операции
      STOP, // остановка машины, завершение работы программы
      LOAD, // LOAD addr - загрузка слова данных в стек из памяти по адресу addr
      STORE, // STORE addr - запись слова данных с вершины стека в память по
             // адресу addr
      BLOAD, // BLOAD addr - загрузка слова данных в стек из памяти по адресу
             // addr
             // + значение на вершине стека
      BSTORE, // BSTORE addr - запись слова данных по адресу addr + значение на
              // вершине стека
      PUSH,   // PUSH n - загрузка в стек константы n
      POP,    // удаление слова с вершины стека
      DUP,    // копирование слова на вершине стека
      ADD, // сложение двух слов на вершине стека и запись результата вместо них
      SUB, // вычитание двух слов на вершине стека и запись результата вместо
           // них
      MULT, // умножение двух слов на вершине стека и запись результата вместо
            // них
      DIV, // деление двух слов на вершине стека и запись результата вместо них
      INVERT,   // изменение знака слова на вершине стека
      COMPARE,  // COMPARE cmp - сравнение двух слов на вершине стека с помощью
                // операции сравнения с кодом cmp
      JUMP,     // JUMP addr - безусловный переход по адресу addr
      JUMP_YES, // JUMP_YES addr - переход по адресу addr, если на вершине стека
                // значение 1
      JUMP_NO,  // JUMP_NO addr - переход по адресу addr, если на вершине стека
                // значение 0
      INPUT, // чтение целого числа со стандартного ввода и загрузка его в стек
      PRINT  // печать на стандартный вывод числа с вершины стека
    };

    // Конструктор для инструкций без аргументов
    explicit command(vm_instruction instruction) :
      instruction_(instruction), arg_(0)
    {}

    // Конструктор для инструкций с одним аргументом
    command(vm_instruction instruction, int arg) :
      instruction_(instruction), arg_(arg)
    {}

    auto instruction() const noexcept { return instruction_; }
    auto arg() const noexcept { return arg_; }

  private:
    vm_instruction instruction_; // Код инструкции
    int arg_;                    // Аргумент инструкции
  };

  void print_command(const command& command, int address, std::ostream& out);

  // Кодогенератор.
  // Назначение кодогенератора:
  // - Формировать программу для виртуальной машины Милана
  // - Отслеживать адрес последней инструкции
  // - Буферизовать программу и печатать ее в указанный поток вывода
  class code_generator
  {
  public:
    code_generator() = default;

    // Добавление инструкции без аргументов в конец программы
    void emit(command::vm_instruction instruction);

    // Добавление инструкции с одним аргументом в конец программы
    void emit(command::vm_instruction instruction, int arg);

    // Запись инструкции без аргументов по указанному адресу
    void emit_at(int address, command::vm_instruction instruction);

    // Запись инструкции с одним аргументом по указанному адресу
    void emit_at(int address, command::vm_instruction instruction, int arg);

    // Получение адреса, непосредственно следующего за последней инструкцией в
    // программе
    int get_current_address();

    // Формирование "пустой" инструкции (NOP) и возврат ее адреса
    int reserve();

    const auto& get_command_buffer() const noexcept { return command_buffer_; }

  private:
    std::vector< command > command_buffer_; // Буфер инструкций
  };

}

#endif
