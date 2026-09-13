#include "parser.hpp"
#include <sstream>
#include "codegen.hpp"
#include "scanner.hpp"
#include "type_utils.hpp"

// Выполняем синтаксический разбор блока program. Если во время разбора не
// обнаруживаем никаких ошибок, то выводим последовательность команд стек-машины

void
cmilan::parser::read_statement_list()
{
  //	  Если список операторов пуст, очередной лексемой будет одна из возможных
  //"закрывающих скобок": END, OD, ELSE, FI. 	  В этом случае результатом
  // разбора будет пустой блок (его список операторов равен null). 	  Если
  // очередная лексема не входит в этот список, то ее мы считаем началом
  // оператора и вызываем метод statement.
  //    Признаком последнего оператора является отсутствие после оператора точки
  //    с запятой.
  if (see(token::T_END) || see(token::T_OD) || see(token::T_ELSE) ||
      see(token::T_FI))
  {
    return;
  }
  else
  {
    bool more = true;
    while (more)
    {
      read_statement();
      more = match(token::T_SEMICOLON);
    }
  }
}

void
cmilan::parser::read_statement()
{
  // Если встречаем переменную, то запоминаем ее адрес или добавляем новую если
  // не встретили. Следующей лексемой должно быть присваивание. Затем идет блок
  // expression, который возвращает значение на вершину стека. Записываем это
  // значение по адресу нашей переменной
  if (see(token::T_IDENTIFIER))
  {
    int var_address = find_or_add_variable(scanner_->get_string_value());
    next();
    must_be(token::T_ASSIGN);
    read_expression();
    codegen_->emit(command::vm_instruction::STORE, var_address);
  }
  // Если встретили IF, то затем должно следовать условие. На вершине стека
  // лежит 1 или 0 в зависимости от выполнения условия. Затем зарезервируем
  // место для условного перехода JUMP_NO к блоку ELSE (переход в случае ложного
  // условия). Адрес перехода станет известным только после того, как будет
  // сгенерирован код для блока THEN.
  else if (match(token::T_IF))
  {
    read_relation();

    int jump_no_address = codegen_->reserve();

    must_be(token::T_THEN);
    read_statement_list();
    if (match(token::T_ELSE))
    {
      // Если есть блок ELSE, то чтобы не выполнять его в случае выполнения
      // THEN, зарезервируем место для команды JUMP в конец этого блока
      int jump_address = codegen_->reserve();
      // Заполним зарезервированное место после проверки условия инструкцией
      // перехода в начало блока ELSE.
      codegen_->emit_at(jump_no_address,
                        command::vm_instruction::JUMP_NO,
                        codegen_->get_current_address());
      read_statement_list();
      // Заполним второй адрес инструкцией перехода в конец условного блока
      // ELSE.
      codegen_->emit_at(jump_address,
                        command::vm_instruction::JUMP,
                        codegen_->get_current_address());
    }
    else
    {
      // Если блок ELSE отсутствует, то в зарезервированный адрес после проверки
      // условия будет записана инструкция условного перехода в конец оператора
      // IF...THEN
      codegen_->emit_at(jump_no_address,
                        command::vm_instruction::JUMP_NO,
                        codegen_->get_current_address());
    }

    must_be(token::T_FI);
  }

  else if (match(token::T_FOR))
  {
    // Определяем, какой синтаксис for используется
    if (see(token::T_LPAREN))
    {
      read_cfor_loop();
    }
    else
    {
      read_for_loop();
    }
  }

  // REPEAT-UNTIL - НОВЫЙ ЦИКЛ
  else if (match(token::T_REPEAT))
  {
    read_repeat_until();
  }

  else if (match(token::T_WHILE))
  {
    // запоминаем адрес начала проверки условия.
    int condition_address = codegen_->get_current_address();
    read_relation();
    // резервируем место под инструкцию условного перехода для выхода из цикла.
    int jump_no_address = codegen_->reserve();
    must_be(token::T_DO);
    read_statement_list();
    must_be(token::T_OD);
    // переходим по адресу проверки условия
    codegen_->emit(command::vm_instruction::JUMP, condition_address);
    // заполняем зарезервированный адрес инструкцией условного перехода на
    // следующий за циклом оператор.
    codegen_->emit_at(jump_no_address,
                      command::vm_instruction::JUMP_NO,
                      codegen_->get_current_address());
  }
  else if (match(token::T_WRITE))
  {
    must_be(token::T_LPAREN);
    read_expression();
    must_be(token::T_RPAREN);
    codegen_->emit(command::vm_instruction::PRINT);
  }
  else
  {
    report_error("statement expected.");
  }
}

void
cmilan::parser::read_expression()
{

  /*
        Арифметическое выражение описывается следующими правилами: <expression>
    -> <term> | <term> + <term> | <term> - <term> При разборе сначала смотрим
    первый терм, затем анализируем очередной символ. Если это '+' или '-',
    удаляем его из потока и разбираем очередное слагаемое (вычитаемое).
    Повторяем проверку и разбор очередного терма, пока не встретим за термом
    символ, отличный от '+' и '-'
    */

  read_term();
  while (see(token::T_ADDOP))
  {
    arithmetic_operation op = scanner_->get_arithmetic_value();
    next();
    read_term();

    if (op == arithmetic_operation::PLUS)
    {
      codegen_->emit(command::vm_instruction::ADD);
    }
    else
    {
      codegen_->emit(command::vm_instruction::SUB);
    }
  }
}

void
cmilan::parser::read_term()
{
  /*
    Терм описывается следующими правилами: <expression> -> <factor> | <factor> +
    <factor> | <factor> - <factor> При разборе сначала смотрим первый множитель,
    затем анализируем очередной символ. Если это '*' или '/', удаляем его из
    потока и разбираем очередное слагаемое (вычитаемое). Повторяем проверку и
    разбор очередного множителя, пока не встретим за ним символ, отличный от '*'
    и '/'
 */
  read_factor();
  while (see(token::T_MULOP))
  {
    arithmetic_operation op = scanner_->get_arithmetic_value();
    next();
    read_factor();

    if (op == arithmetic_operation::MULTIPLY)
    {
      codegen_->emit(command::vm_instruction::MULT);
    }
    else
    {
      codegen_->emit(command::vm_instruction::DIV);
    }
  }
}

void
cmilan::parser::read_factor()
{
  /*
    Множитель описывается следующими правилами:
    <factor> -> number | identifier | -<factor> | (<expression>) | READ
  */
  if (see(token::T_NUMBER))
  {
    int value = scanner_->get_int_value();
    next();
    codegen_->emit(command::vm_instruction::PUSH, value);
    // Если встретили число, то преобразуем его в целое и записываем на вершину
    // стека
  }
  else if (see(token::T_IDENTIFIER))
  {
    int var_address = find_or_add_variable(scanner_->get_string_value());
    next();
    codegen_->emit(command::vm_instruction::LOAD, var_address);
    // Если встретили переменную, то выгружаем значение, лежащее по ее адресу,
    // на вершину стека
  }
  else if (see(token::T_ADDOP) &&
           scanner_->get_arithmetic_value() == arithmetic_operation::MINUS)
  {
    next();
    read_factor();
    codegen_->emit(command::vm_instruction::INVERT);
    // Если встретили знак "-", и за ним <factor> то инвертируем значение,
    // лежащее на вершине стека
  }
  else if (match(token::T_LPAREN))
  {
    read_expression();
    must_be(token::T_RPAREN);
    // Если встретили открывающую скобку, тогда следом может идти любое
    // арифметическое выражение и обязательно закрывающая скобка.
  }
  else if (match(token::T_READ))
  {
    codegen_->emit(command::vm_instruction::INPUT);
    // Если встретили зарезервированное слово READ, то записываем на вершину
    // стека идет запись со стандартного ввода
  }
  else
  {
    report_error("expression expected.");
  }
}

void
cmilan::parser::read_relation()
{
  // Условие сравнивает два выражения по какому-либо из знаков. Каждый знак
  // имеет свой номер. В зависимости от результата сравнения на вершине стека
  // окажется 0 или 1.
  read_expression();
  if (see(token::T_CMP))
  {
    cmp_operation cmp = scanner_->get_cmp_value();
    next();
    read_expression();

    auto op_code = cmilan::to_underlying_type(cmp);
    codegen_->emit(command::vm_instruction::COMPARE, op_code);
  }
  else
  {
    report_error("comparison operator expected.");
  }
}

void
cmilan::parser::read_cfor_loop()
{
  // Синтаксис: for(<инициализация>; <условие>; <инкремент>) do <операторы> od

  must_be(token::T_LPAREN);

  // 1. Инициализация
  read_statement(); // например, i := 1
  must_be(token::T_SEMICOLON);

  // 2. Запоминаем начало проверки условия
  int condition_address = codegen_->get_current_address();

  // 3. Проверяем условие
  read_relation();                        // например, i <= 10
  int exit_address = codegen_->reserve(); // выход если условие ложно

  must_be(token::T_SEMICOLON);

  // 4. Запоминаем код инкремента (с помощью отдельного парсера)
  struct IncrementCode
  {
    command::vm_instruction instr;
    int arg;
    bool has_arg;
  };
  std::vector< IncrementCode > increment_instructions;

  // Сохраняем текущий кодогенератор и создаём временный
  auto saved_codegen = std::move(codegen_);
  codegen_ = std::make_unique< code_generator >();

  // Разбираем инкремент во временный буфер
  read_statement(); // например, i := i + 1
  must_be(token::T_RPAREN);
  must_be(token::T_DO);

  // Сохраняем инструкции инкремента
  for (const auto& cmd: codegen_->get_command_buffer())
  {
    increment_instructions.push_back({ cmd.instruction(), cmd.arg(), true });
  }

  // Восстанавливаем основной кодогенератор
  codegen_ = std::move(saved_codegen);

  // 5. Генерируем тело цикла
  int body_address = codegen_->get_current_address();
  read_statement_list();
  must_be(token::T_OD);

  // 6. Генерируем инкремент
  for (const auto& inc: increment_instructions)
  {
    if (inc.has_arg)
    {
      codegen_->emit(inc.instr, inc.arg);
    }
    else
    {
      codegen_->emit(inc.instr);
    }
  }

  // 7. Переход на проверку условия
  codegen_->emit(command::vm_instruction::JUMP, condition_address);

  // 8. Заполняем выход из цикла
  codegen_->emit_at(exit_address,
                    command::vm_instruction::JUMP_NO,
                    codegen_->get_current_address());
}

void
cmilan::parser::read_for_loop()
{
  // Синтаксис: for <переменная> := <выражение> to <выражение> step
  // do <операторы> od

  // 1. Получаем переменную цикла
  if (!see(token::T_IDENTIFIER))
  {
    report_error("variable name expected after 'for'");
    recover(token::T_DO);
    return;
  }

  std::string var_name = scanner_->get_string_value();
  int var_address = find_or_add_variable(var_name);
  next(); // пропускаем идентификатор

  // 2. Присваивание начального значения
  must_be(token::T_ASSIGN);
  read_expression();
  codegen_->emit(command::vm_instruction::STORE, var_address);

  // 3. Сохраняем конечное значение
  must_be(token::T_TO);
  read_expression(); // конечное выражение
  // Дублируем, так как понадобится для сравнения
  codegen_->emit(command::vm_instruction::DUP);
  // Сохраняем конечное значение
  int to_var = find_or_add_variable("__for_to_temp");
  codegen_->emit(command::vm_instruction::STORE, to_var);

  // 4. Сохраняем шаг
  must_be(token::T_STEP);
  read_expression(); // шаг
  int step_var = find_or_add_variable("__for_step_temp");
  codegen_->emit(command::vm_instruction::STORE, step_var);

  // 5. Начало цикла - проверка условия
  int loop_check_address = codegen_->get_current_address();

  // Загружаем текущее значение переменной для сравнения
  codegen_->emit(command::vm_instruction::LOAD, var_address);
  // Загружаем конечное значение
  codegen_->emit(command::vm_instruction::LOAD, to_var);

  // Проверяем знак шага, чтобы выбрать правильное сравнение
  // Загружаем шаг
  codegen_->emit(command::vm_instruction::LOAD, step_var);
  // Проверяем, больше ли шаг нуля
  codegen_->emit(command::vm_instruction::PUSH, 0);
  codegen_->emit(command::vm_instruction::COMPARE,
                 cmilan::to_underlying_type(cmp_operation::GT));

  // В зависимости от знака шага выбираем сравнение
  int positive_branch = codegen_->reserve(); // JUMP_YES если шаг > 0
  int negative_branch = codegen_->get_current_address();

  // Шаг отрицательный - используем >=
  codegen_->emit(command::vm_instruction::LOAD, var_address);
  codegen_->emit(command::vm_instruction::LOAD, to_var);
  codegen_->emit(command::vm_instruction::COMPARE,
                 cmilan::to_underlying_type(cmp_operation::GE));
  int exit_negative = codegen_->reserve(); // JUMP_NO для отрицательного шага
  int skip_positive =
    codegen_->reserve(); // JUMP для пропуска положительной ветки

  // Заполняем positive_branch
  codegen_->emit_at(positive_branch,
                    command::vm_instruction::JUMP_YES,
                    codegen_->get_current_address() +
                      2); // +2 для пропуска JUMP

  // Шаг положительный - используем <=
  codegen_->emit(command::vm_instruction::LOAD, var_address);
  codegen_->emit(command::vm_instruction::LOAD, to_var);
  codegen_->emit(command::vm_instruction::COMPARE,
                 cmilan::to_underlying_type(cmp_operation::LE));
  int exit_positive = codegen_->reserve(); // JUMP_NO для положительного шага

  // Заполняем skip_positive
  codegen_->emit_at(skip_positive,
                    command::vm_instruction::JUMP,
                    codegen_->get_current_address() + 1);

  // Заполняем выходы
  int exit_address = codegen_->reserve(); // общий выход

  // 6. Тело цикла
  must_be(token::T_DO);
  read_statement_list();

  // 7. Инкремент переменной
  codegen_->emit(command::vm_instruction::LOAD, var_address);
  codegen_->emit(command::vm_instruction::LOAD, step_var);
  codegen_->emit(command::vm_instruction::ADD);
  codegen_->emit(command::vm_instruction::STORE, var_address);

  // 8. Переход к проверке условия
  codegen_->emit(command::vm_instruction::JUMP, loop_check_address);

  // 9. Заполняем выходы
  must_be(token::T_OD);

  codegen_->emit_at(exit_positive,
                    command::vm_instruction::JUMP_NO,
                    codegen_->get_current_address());
  codegen_->emit_at(exit_negative,
                    command::vm_instruction::JUMP_NO,
                    codegen_->get_current_address());
  codegen_->emit_at(exit_address, command::vm_instruction::NOP, 0);
}

void
cmilan::parser::read_repeat_until()
{
  int body_start = codegen_->get_current_address();
  read_statement_list();
  must_be(token::T_UNTIL);
  read_relation();

  // Если условие истинно (1) - выходим (JUMP_YES)
  // Если ложно (0) - продолжаем (не переходим)
  codegen_->emit(command::vm_instruction::JUMP_YES,
                 codegen_->get_current_address() + 2);       // выход
  codegen_->emit(command::vm_instruction::JUMP, body_start); // повтор
}

int
cmilan::parser::find_or_add_variable(const std::string& var)
{
  var_table::iterator it = variables_.find(var);
  if (it == variables_.end())
  {
    variables_[var] = last_var_number_;
    return last_var_number_++;
  }
  else
  {
    return it->second;
  }
}

void
cmilan::parser::must_be(token t)
{
  if (!match(t))
  {
    error_ = true;

    // Подготовим сообщение об ошибке
    std::ostringstream msg;
    msg << to_string(scanner_->get_token()) << " found while " << to_string(t)
        << " expected.";
    report_error(msg.str());

    // Попытка восстановления после ошибки.
    recover(t);
  }
}

void
cmilan::parser::recover(token t)
{
  while (!see(t) && !see(token::T_EOF))
  {
    next();
  }

  if (see(t))
  {
    next();
  }
}
