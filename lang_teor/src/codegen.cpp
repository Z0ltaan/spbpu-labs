#include "codegen.hpp"

void
cmilan::print_command(const command& command, int address, std::ostream& out)
{
  out << address << ":\t";
  switch (command.instruction())
  {
    case command::vm_instruction::NOP:
      out << "NOP";
      break;

    case command::vm_instruction::STOP:
      out << "STOP";
      break;

    case command::vm_instruction::LOAD:
      out << "LOAD\t" << command.arg();
      break;

    case command::vm_instruction::STORE:
      out << "STORE\t" << command.arg();
      break;

    case command::vm_instruction::BLOAD:
      out << "BLOAD\t" << command.arg();
      break;

    case command::vm_instruction::BSTORE:
      out << "BSTORE\t" << command.arg();
      break;

    case command::vm_instruction::PUSH:
      out << "PUSH\t" << command.arg();
      break;

    case command::vm_instruction::POP:
      out << "POP";
      break;

    case command::vm_instruction::DUP:
      out << "DUP";
      break;

    case command::vm_instruction::ADD:
      out << "ADD";
      break;

    case command::vm_instruction::SUB:
      out << "SUB";
      break;

    case command::vm_instruction::MULT:
      out << "MULT";
      break;

    case command::vm_instruction::DIV:
      out << "DIV";
      break;

    case command::vm_instruction::INVERT:
      out << "INVERT";
      break;

    case command::vm_instruction::COMPARE:
      out << "COMPARE\t" << command.arg();
      break;

    case command::vm_instruction::JUMP:
      out << "JUMP\t" << command.arg();
      break;

    case command::vm_instruction::JUMP_YES:
      out << "JUMP_YES\t" << command.arg();
      break;

    case command::vm_instruction::JUMP_NO:
      out << "JUMP_NO\t" << command.arg();
      break;

    case command::vm_instruction::INPUT:
      out << "INPUT";
      break;

    case command::vm_instruction::PRINT:
      out << "PRINT";
      break;
  }

  out << std::endl;
}

void
cmilan::code_generator::emit(command::vm_instruction instruction)
{
  command_buffer_.push_back(command(instruction));
}

void
cmilan::code_generator::emit(command::vm_instruction instruction, int arg)
{
  command_buffer_.push_back(command(instruction, arg));
}

void
cmilan::code_generator::emit_at(int address,
                                command::vm_instruction instruction)
{
  command_buffer_.at(address) = command(instruction);
}

void
cmilan::code_generator::emit_at(int address,
                                command::vm_instruction instruction,
                                int arg)
{
  command_buffer_.at(address) = command(instruction, arg);
}

int
cmilan::code_generator::get_current_address()
{
  return command_buffer_.size();
}

int
cmilan::code_generator::reserve()
{
  emit(command::vm_instruction::NOP);
  return command_buffer_.size() - 1;
}
