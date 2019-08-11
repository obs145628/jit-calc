#pragma once

#include <fstream>

class Logger
{

public:

  static Logger& instance()
  {
    static Logger res;
    return res;
  }
  
  std::ostream& os() { return _os; }

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

private:
  Logger()
    : _os("mlk.logs")
  {}

  std::ofstream _os;
  
};

#define LOG(X) (Logger::instance().os() << X << std::endl)
