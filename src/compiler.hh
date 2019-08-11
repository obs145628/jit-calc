#pragma once

#include "x64-writter.hh"
#include "ir.hh"

class Compiler
{
public:
  Compiler(unsigned char* buf);

  void compile_code(const IRCode& code);
  void compile_ins(const IRIns& ins);

private:
  X64Writter _xw;
  const std::vector<int> _regs;
};
