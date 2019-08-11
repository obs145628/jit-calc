#include "parser.hh"
#include <iostream>
#include <sstream>
#include <stdexcept>

/**
 * grammar:
 * E: E2 <EOF>
 * E2: E1 (+|- E1)*
 * E1: E0 (*|/|% E0)*
 * E0: num | '(' E2 ')'
 */

namespace
{

  void skip_wspaces(std::istream& is)
  {
    while (true)
    {
      int c = is.get();
      if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
      {
	is.unget();
	break;
      }
    }
  }

  void get_eof(std::istream& is)
  {
    skip_wspaces(is);
    int c = is.get();
    if (c != EOF)
      throw std::runtime_error {"Parser error: Expected <EOF>"};
  }
  
  num_t get_num(std::istream& is)
  {
    skip_wspaces(is);
    num_t val = 0;
    bool def = false;
    
    while (true)
    {
      int c = is.get();
      if (c < '0' || c > '9')
      {
	is.unget();
	break;
      }
      def = true;
      val = 10 * val + static_cast<num_t>(c - '0'); 
    }

    if (!def)
      throw std::runtime_error {"Parser error: expected number"};
    return val;
  }

  char try_get_char(std::istream& is, const char* ops)
  {
    skip_wspaces(is);
    int c = is.get();

    for (int i = 0; ops[i]; ++i)
      if (ops[i] == c)
	return c;

    is.unget();
    return 0;
  }

  ExprTreePtr e0(std::istream& is);
  ExprTreePtr e1(std::istream& is);
  ExprTreePtr e2(std::istream& is);
  ExprTreePtr e(std::istream& is);

  ExprTreePtr e0(std::istream& is)
  {

    char lp = try_get_char(is, "(");
    if (!lp)
    {
      num_t x = get_num(is);
      return std::make_unique<ExprTree>(x);
    }

    auto res = e2(is);
    if (!try_get_char(is, ")"))
      throw std::runtime_error {"Parser error: expected ')'"};
    return res;
  }

  ExprTreePtr e1(std::istream& is)
  {
    auto left = e0(is);

    while (true)
    {
      char op = try_get_char(is, "*/%");
      if (!op)
	break;

      auto right = e0(is);
      if (op == '*')
	left = std::make_unique<ExprTree>(std::move(left), std::move(right), OpType::I64_MUL);
      else if (op == '/')
	left = std::make_unique<ExprTree>(std::move(left), std::move(right), OpType::I64_DIV);
      else if (op == '%')
	left = std::make_unique<ExprTree>(std::move(left), std::move(right), OpType::I64_MOD);
      else
	throw std::runtime_error {"Parser: Internal error"};
    }

    return left;
  }

  ExprTreePtr e2(std::istream& is)
  {
    auto left = e1(is);

    while (true)
    {
      char op = try_get_char(is, "+-");
      if (!op)
	break;

      auto right = e1(is);
      if (op == '+')
	left = std::make_unique<ExprTree>(std::move(left), std::move(right), OpType::I64_ADD);
      else if (op == '-')
	left = std::make_unique<ExprTree>(std::move(left), std::move(right), OpType::I64_SUB);
      else
	throw std::runtime_error {"Parser: Internal error"};
    }

    return left;
  }

  ExprTreePtr e(std::istream& is)
  {
    auto res = e2(is);
    get_eof(is);
    return res;
  }
}


ExprTreePtr parse_line(const std::string& str)
{
  std::istringstream is(str);
  return e(is);
}



void dump_expr_tree(const ExprTreePtr& expr)
{
  if (!expr->left && !expr->right)
  {
    std::cout << expr->val;
    return;
  }

  std::cout << "(";
  dump_expr_tree(expr->left);
  std::cout << ")";

  if (expr->op == OpType::I64_ADD)
    std::cout << " + ";
  else if (expr->op == OpType::I64_SUB)
    std::cout << " - ";
  else if (expr->op == OpType::I64_MUL)
    std::cout << " * ";
  else if (expr->op == OpType::I64_DIV)
    std::cout << " / ";
  else if (expr->op == OpType::I64_MOD)
    std::cout << " % ";

  std::cout << "(";
  dump_expr_tree(expr->right);
  std::cout << ")";
}
