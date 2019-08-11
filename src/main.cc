#include <iostream>
#include <string>

#include "compiler.hh"
#include "exec-memory.hh"
#include "ir.hh"
#include "parser.hh"


std::int64_t run_exp(const std::string& str, bool interpret = false)
{
  using fn_type = int64_t (*)();
  ExprTreePtr exp_tree = parse_line(str);
  IRGen gen;
  gen.generate(exp_tree);
  auto code_ir = gen.get_code();
  dump_ir(code_ir);
  if (interpret)
    return eval_ir(code_ir);

  ExecMemory ex_mem(4096);
  Compiler cc(ex_mem.get());
  cc.compile_code(code_ir);
  auto fn = ex_mem.get<fn_type> ();
  return fn();
}


int main(int argc, char** argv)
{
  if (argc > 1)
  {
    std::cout << run_exp(argv[1]) << std::endl;
    return 0;
  }

  
  while (true)
  {
    std::string str;
    std::cout << "> ";
    std::getline(std::cin, str);
    if (str.empty())
      break;

    auto res = run_exp(str);
    std::cout << "> " << res << std::endl;
  }
  
  return 0; 
}
