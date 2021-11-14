#pragma once
#include <string>
#include "Parser.h"
#include "FunctionManager.h"

class Calculator
{
public:
  Calculator() {};
  double calculate(const std::string& expression);

private:
  Parser m_parser;
  FunctionManager m_fManager;
  void eval(std::stack<double>& stack, Token* token);
};

