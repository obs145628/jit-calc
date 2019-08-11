#pragma once

#include <cstddef>

class ExecMemory
{

public:

  ExecMemory(std::size_t len);
  ~ExecMemory();
  ExecMemory(const ExecMemory&) = delete;
  ExecMemory& operator=(const ExecMemory&) = delete;

  template <class T = unsigned char*>
  T get()
  {
    return reinterpret_cast<T>(_ptr_align);
  }

private:
  char* _ptr;
  char* _ptr_align;
};
