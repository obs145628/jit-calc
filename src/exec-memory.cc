#include "exec-memory.hh"

#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

ExecMemory::ExecMemory(std::size_t len)
{
  std::size_t pagesize = getpagesize();
  _ptr = new char[pagesize + len];
  _ptr_align = reinterpret_cast<char*>(  (reinterpret_cast<std::size_t> (_ptr + pagesize - 1)
					  & ~(pagesize - 1)) );

  if (mprotect(_ptr_align, len, PROT_READ|PROT_EXEC|PROT_WRITE))
    throw std::runtime_error {"failed to change memory right accesses"};
}

ExecMemory::~ExecMemory()
{
  delete[] _ptr;
}

