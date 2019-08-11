#include "compiler.hh"

#include <cassert>

Compiler::Compiler(unsigned char* buf)
  : _xw(buf)
  , _regs({
      X64Writter::REG_RBX,
	X64Writter::REG_RCX,
	X64Writter::REG_RSI,
	X64Writter::REG_RDI,
	X64Writter::REG_R8,
	X64Writter::REG_R9,
	X64Writter::REG_R10,
	X64Writter::REG_R11,
	X64Writter::REG_R12,
	X64Writter::REG_R13,
	X64Writter::REG_R14,
	X64Writter::REG_R15
    })
{}


void Compiler::compile_ins(const IRIns& ins)
{
  if (ins.ins == IRInsType::I64_MOV)
  {
    _xw.ins_mov_64reg_const(_regs[ins.args[0]], ins.args[1]);
  }

  else if (ins.ins == IRInsType::I64_ADD)
  {
    assert(ins.args[0] == ins.args[1]);
    _xw.ins_add_64reg(_regs[ins.args[0]], _regs[ins.args[2]]);
  }

  else if (ins.ins == IRInsType::I64_SUB)
  {
    assert(ins.args[0] == ins.args[1]);
    _xw.ins_sub_64reg(_regs[ins.args[0]], _regs[ins.args[2]]);
  }

  else if (ins.ins == IRInsType::I64_MUL)
  {
    assert(ins.args[0] == ins.args[1]);
    _xw.ins_imul_64reg(_regs[ins.args[0]], _regs[ins.args[2]]);
  }

  else if (ins.ins == IRInsType::I64_DIV)
  {
    _xw.ins_mov_64reg_const(0, 0);
    _xw.ins_add_64reg(0, _regs[ins.args[1]]);
    _xw.ins_cqto();
    _xw.ins_idiv_64reg(_regs[ins.args[2]]);
    _xw.ins_mov_64reg_const(_regs[ins.args[0]], 0);
    _xw.ins_add_64reg(_regs[ins.args[0]], 0);
  }

  else if (ins.ins == IRInsType::I64_MOD)
  {
    _xw.ins_mov_64reg_const(0, 0);
    _xw.ins_add_64reg(0, _regs[ins.args[1]]);
    _xw.ins_cqto();
    _xw.ins_idiv_64reg(_regs[ins.args[2]]);
    _xw.ins_mov_64reg_const(_regs[ins.args[0]], 0);
    _xw.ins_add_64reg(_regs[ins.args[0]], X64Writter::REG_RDX);
  }
}

void Compiler::compile_code(const IRCode& code)
{
  for (const auto& ins : code.ins)
    compile_ins(ins);

  int last = _regs[code.ins.back().args[0]];
  if (last != X64Writter::REG_RAX)
  {
    _xw.ins_mov_64reg_const(X64Writter::REG_RAX, 0);
    _xw.ins_add_64reg(X64Writter::REG_RAX, last);
  }

  _xw.ins_retq();
}
