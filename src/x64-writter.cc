#include "x64-writter.hh"
#include <cstring>

#include "logger.hh"

namespace
{
  constexpr const char* REG_NAMES[] =
    {
      "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp",
      "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
    };
}

void X64Writter::ins_mov_64reg_const(int reg_dst, std::int64_t val)
{
  LOG("X64: mov %" << REG_NAMES[reg_dst] << ", " << val);
  static const int reg_base[] = {
    0xb8, 0xbb, 0xb9, 0xba, 0xbe, 0xbf, 0xbd, 0xbc
  };

  bool only4 = !(val & (0xffffffffL << 32));

  if (only4)
  {

    if (reg_dst < 8)
      *(_buf++) = reg_base[reg_dst];
    else
    {
      *(_buf++) = 0x41;
      *(_buf++) = 0xb0 | reg_dst;
    }

    std::memcpy(_buf, &val, 4);
    _buf += 4;
  }

  else
  {
    if (reg_dst < 8)
    {
      *(_buf++) = 0x48;
      *(_buf++) = reg_base[reg_dst];
    }
    else
    {
      *(_buf++) = 0x49;
      *(_buf++) = 0xb0 | reg_dst;
    }

    std::memcpy(_buf, &val, 8);
    _buf += 8;
  }


}

void X64Writter::ins_add_64reg(int reg_a, int reg_b)
{
  LOG("X64: add %" << REG_NAMES[reg_a] << ", %" << REG_NAMES[reg_b]);
  static const int reg_vals[] = {0, 3, 1, 2, 6, 7, 5, 4};

  if (reg_a < 8 && reg_b < 8)
  {
    *(_buf++) = 0x48;
    *(_buf++) = 0x01;
    *(_buf++) = 0xc0 | (reg_vals[reg_b] << 3) | reg_vals[reg_a];
  }

  else if (reg_a < 8 && reg_b >= 8)
  {
    *(_buf++) = 0x4c;
    *(_buf++) = 0x01;
    *(_buf++) = 0xc0 | ((reg_b - 8) << 3) | reg_vals[reg_a];
  }

  else if (reg_a >= 8 && reg_b < 8)
  {
    *(_buf++) = 0x49;
    *(_buf++) = 0x01;
    *(_buf++) = 0xc0 | (reg_vals[reg_b] << 3) | (reg_a - 8);
  }

  else if (reg_a >= 8 && reg_b >= 8)
  {
    *(_buf++) = 0x4d;
    *(_buf++) = 0x01;
    *(_buf++) = 0xc0 | ((reg_b - 8) << 3) | (reg_a - 8);
  }  
}


void X64Writter::ins_sub_64reg(int reg_a, int reg_b)
{
  LOG("X64: sub %" << REG_NAMES[reg_a] << ", %" << REG_NAMES[reg_b]);
  static const int reg_vals[] = {0, 3, 1, 2, 6, 7, 5, 4};

  if (reg_a < 8 && reg_b < 8)
  {
    *(_buf++) = 0x48;
    *(_buf++) = 0x29;
    *(_buf++) = 0xc0 | (reg_vals[reg_b] << 3) | reg_vals[reg_a];
  }

  else if (reg_a < 8 && reg_b >= 8)
  {
    *(_buf++) = 0x4c;
    *(_buf++) = 0x29;
    *(_buf++) = 0xc0 | ((reg_b - 8) << 3) | reg_vals[reg_a];
  }

  else if (reg_a >= 8 && reg_b < 8)
  {
    *(_buf++) = 0x49;
    *(_buf++) = 0x29;
    *(_buf++) = 0xc0 | (reg_vals[reg_b] << 3) | (reg_a - 8);
  }

  else if (reg_a >= 8 && reg_b >= 8)
  {
    *(_buf++) = 0x4d;
    *(_buf++) = 0x29;
    *(_buf++) = 0xc0 | ((reg_b - 8) << 3) | (reg_a - 8);
  }  
}

void X64Writter::ins_imul_64reg(int reg_a, int reg_b)
{
  LOG("X64: imul %" << REG_NAMES[reg_a] << ", %" << REG_NAMES[reg_b]);
  static const int reg_vals[] = {0, 3, 1, 2, 6, 7, 5, 4};

  if (reg_a < 8 && reg_b < 8)
  {
    *(_buf++) = 0x48;
    *(_buf++) = 0x0f;
    *(_buf++) = 0xaf;
    *(_buf++) = 0xc0 | (reg_vals[reg_a] << 3) | reg_vals[reg_b];
  }

  else if (reg_a >= 8 && reg_b <= 8)
  {
    *(_buf++) = 0x4c;
    *(_buf++) = 0x0f;
    *(_buf++) = 0xaf;
    *(_buf++) = 0xc0 | ((reg_a - 8) << 3) | reg_vals[reg_b];
  }

  else if (reg_a < 8 && reg_b >= 8)
  {
    *(_buf++) = 0x49;
    *(_buf++) = 0x0f;
    *(_buf++) = 0xaf;
    *(_buf++) = 0xc0 | (reg_vals[reg_a] << 3) | (reg_b - 8);
  }

  else if (reg_a >= 8 && reg_b >= 8)
  {
    *(_buf++) = 0x4d;
    *(_buf++) = 0x0f;
    *(_buf++) = 0xaf;
    *(_buf++) = 0xc0 | ((reg_a - 8) << 3) | (reg_b - 8);
  }  
}

void X64Writter::ins_idiv_64reg(int reg_a)
{
  LOG("X64: idiv %" << REG_NAMES[reg_a]);
  static const int reg_vals[] = {0, 3, 1, 2, 6, 7, 5, 4};

  
  if (reg_a < 8)
  {
    *(_buf++) = 0x48;
    *(_buf++) = 0xf7;
    *(_buf++) = 0xf8 | reg_vals[reg_a];
  }

  else
  {
    *(_buf++) = 0x49;
    *(_buf++) = 0xf7;
    *(_buf++) = 0xf8 | (reg_a - 8);
  }
}

void X64Writter::ins_retq()
{
  LOG("X64: retq");
  *(_buf++) = 0xc3;
}


void X64Writter::ins_cqto()
{
  LOG("X64: cqto");
  *(_buf++) = 0x48;
  *(_buf++) = 0x99;
}
