#pragma once

#include <cstdint>

class X64Writter
{
public:

  static constexpr int REG_RAX = 0;
  static constexpr int REG_RBX = 1;
  static constexpr int REG_RCX = 2;
  static constexpr int REG_RDX = 3;
  static constexpr int REG_RSI = 4;
  static constexpr int REG_RDI = 5;
  static constexpr int REG_RBP = 6;
  static constexpr int REG_RSP = 7;
  static constexpr int REG_R8 = 8;
  static constexpr int REG_R9 = 9;
  static constexpr int REG_R10 = 10;
  static constexpr int REG_R11 = 11;
  static constexpr int REG_R12 = 12;
  static constexpr int REG_R13 = 13;
  static constexpr int REG_R14 = 14;
  static constexpr int REG_R15 = 15;
  static constexpr int NB_REGS = 16;
  
  X64Writter(unsigned char* buf)
    : _buf(buf)
  {}


  ///dst <- val
  void ins_mov_64reg_const(int reg_dst, std::int64_t val);

  ///a = a + b
  void ins_add_64reg(int reg_a, int reg_b);

  ///a = a - b
  void ins_sub_64reg(int reg_a, int reg_b);

  ///a = a * b
  void ins_imul_64reg(int reg_a, int reg_b);

  //rax = rax / a
  void ins_idiv_64reg(int reg_a);
  
  void ins_retq();

  //sign extend rax into rdx
  void ins_cqto();

  

private:
  unsigned char* _buf;
};
