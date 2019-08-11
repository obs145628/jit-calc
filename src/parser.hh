#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

enum class OpType
{
  I64_ADD,
  I64_SUB,
  I64_MUL,
  I64_DIV,
  I64_MOD
};

using num_t = std::int64_t;

struct ExprTree;
using ExprTreePtr = std::unique_ptr<ExprTree>;

struct ExprTree
{
  ExprTreePtr left;
  ExprTreePtr right;
  OpType op;
  num_t val;

  ExprTree(ExprTreePtr&& left,
	  ExprTreePtr&& right,
	  OpType op)
    : left(std::move(left))
    , right(std::move(right))
    , op(op)
  {}

  ExprTree(num_t val)
    : val(val)
  {}
  
};

ExprTreePtr parse_line(const std::string& str);

void dump_expr_tree(const ExprTreePtr& expr);
