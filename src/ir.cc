#include "ir.hh"
#include <cassert>
#include "logger.hh"

void IRGen::generate(const ExprTreePtr& ptr)
{
  _gen(ptr);
}
  
const IRCode& IRGen::get_code() const
{
  return _code;
}
  


int IRGen::_gen(const ExprTreePtr& ptr)
{
  //assign
  if (!ptr->left)
  {
    int reg = _alloc_reg();
    IRIns ins;
    ins.ins = IRInsType::I64_MOV;
    ins.args[0] = reg;
    ins.args[1] = ptr->val;
    _code.ins.push_back(ins);
    return reg;
  }

  int left = _gen(ptr->left);
  int right = _gen(ptr->right);
  _free_reg(right);

  IRIns ins;

  if (ptr->op == OpType::I64_ADD)
    ins.ins = IRInsType::I64_ADD;
  else if (ptr->op == OpType::I64_SUB)
    ins.ins = IRInsType::I64_SUB;
  else if (ptr->op == OpType::I64_MUL)
    ins.ins = IRInsType::I64_MUL;
  else if (ptr->op == OpType::I64_DIV)
    ins.ins = IRInsType::I64_DIV;
  else if (ptr->op == OpType::I64_MOD)
    ins.ins = IRInsType::I64_MOD;
  
  ins.args[0] = left;
  ins.args[1] = left;
  ins.args[2] = right;
  _code.ins.push_back(ins);
  return left;
}

int IRGen::_alloc_reg()
{
  size_t res = 0;
  for (res = 0; res < _regs.size() && _regs[res]; ++res)
    ;

  if (res == _regs.size())
    _regs.push_back(1);
  else
    _regs[res] = 1;

  return static_cast<int>(res);
}

void IRGen::_free_reg(int reg)
{
  size_t pos = reg;
  assert(pos < _regs.size() && _regs[pos] == 1);
  _regs[pos] = 0;
}


void dump_ir(const IRCode& code)
{
  LOG("IR_DUMP:");
  for (const auto& ins : code.ins)
  {
    if (ins.ins == IRInsType::I64_MOV)
      LOG("i64_mov %" << ins.args[0] << ", " << ins.args[1]);
    else if (ins.ins == IRInsType::I64_ADD)
      LOG("i64_add %" << ins.args[0] << ", %" << ins.args[1] << ", %" << ins.args[2]);
    else if (ins.ins == IRInsType::I64_SUB)
      LOG("i64_sub %" << ins.args[0] << ", %" << ins.args[1] << ", %" << ins.args[2]);
    else if (ins.ins == IRInsType::I64_MUL)
      LOG("i64_mul %" << ins.args[0] << ", %" << ins.args[1] << ", %" << ins.args[2]);
    else if (ins.ins == IRInsType::I64_DIV)
      LOG("i64_div %" << ins.args[0] << ", %" << ins.args[1] << ", %" << ins.args[2]);
    else if (ins.ins == IRInsType::I64_MOD)
      LOG("i64_mod %" << ins.args[0] << ", %" << ins.args[1] << ", %" << ins.args[2]);
  }

  LOG("");
}

std::int64_t eval_ir(const IRCode& code)
{

  std::vector<std::int64_t> data;

  for (const auto& ins : code.ins)
  {
    
    if (ins.ins == IRInsType::I64_MOV)
    {
      if (data.size() <= static_cast<size_t>(ins.args[0]))
	data.resize(static_cast<size_t>(ins.args[0]) + 1);
      data.at(ins.args[0]) = ins.args[1];
    }

    else if (ins.ins == IRInsType::I64_ADD)
    {
      if (data.size() <= static_cast<size_t>(ins.args[0]))
	data.resize(static_cast<size_t>(ins.args[0]) + 1);
      data.at(ins.args[0]) = data.at(ins.args[1]) + data.at(ins.args[2]);
    }
    
    else if (ins.ins == IRInsType::I64_SUB)
    {
      if (data.size() <= static_cast<size_t>(ins.args[0]))
	data.resize(static_cast<size_t>(ins.args[0]) + 1);
      data.at(ins.args[0]) = data.at(ins.args[1]) - data.at(ins.args[2]);
    }

    else if (ins.ins == IRInsType::I64_MUL)
    {
      if (data.size() <= static_cast<size_t>(ins.args[0]))
	data.resize(static_cast<size_t>(ins.args[0]) + 1);
      data.at(ins.args[0]) = data.at(ins.args[1]) * data.at(ins.args[2]);
    }
    
    else if (ins.ins == IRInsType::I64_DIV)
    {
      if (data.size() <= static_cast<size_t>(ins.args[0]))
	data.resize(static_cast<size_t>(ins.args[0]) + 1);
      data.at(ins.args[0]) = data.at(ins.args[1]) / data.at(ins.args[2]);
    }

    else if (ins.ins == IRInsType::I64_MOD)
    {
      if (data.size() <= static_cast<size_t>(ins.args[0]))
	data.resize(static_cast<size_t>(ins.args[0]) + 1);
      data.at(ins.args[0]) = data.at(ins.args[1]) % data.at(ins.args[2]);
    }
  }

  return data.at(0);
}
