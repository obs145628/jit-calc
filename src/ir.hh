#pragma once

#include <cstdint>
#include <vector>
#include "parser.hh"

enum class IRInsType
{
  I64_MOV,
  I64_ADD,
  I64_SUB,
  I64_MUL,
  I64_DIV,
  I64_MOD
};


struct IRIns
{
  IRInsType ins;
  std::int64_t args[3];
};


struct IRCode
{
  std::vector<IRIns> ins;
};


class IRGen
{
public:

  void generate(const ExprTreePtr& ptr);
  
  const IRCode& get_code() const;
  
private:

  int _gen(const ExprTreePtr& ptr);

  int _alloc_reg();
  void _free_reg(int reg);
  
  IRCode _code;
  std::vector<int> _regs;
};

void dump_ir(const IRCode& code);

std::int64_t eval_ir(const IRCode& code);
