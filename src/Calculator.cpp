#include "Calculator.h"
#include <iostream>

double Calculator::calculate(const std::string& expression)
{
  std::queue<Token*> tokens = m_parser.Parse(expression);
  std::stack<double> numbers;
  while (!tokens.empty())
  {
    Token* tmp = tokens.front();
    tokens.pop();
    switch (tmp->type)
    {
    case Token::Type::number:
      numbers.push(((TokenNumber*)tmp)->value);
      break;
    case Token::Type::operation:
    case Token::Type::function:
      m_fManager.eval(((TokenFunction*)tmp)->name, numbers);
    }
  }

  if (numbers.size() > 1)
    throw std::exception("missing operations");
  if (numbers.empty())
    throw std::exception("no values");
  return numbers.top();
  
  while (tokens.size())
  {
    auto token = tokens.front();
    if (token->type == Token::Type::number)
      std::cout << "number: "<<((TokenNumber*)token)->value << std::endl;
    if (token->type == Token::Type::operation)
      std::cout << "operation: " << ((TokenOperation*)token)->name << std::endl;
    if (token->type == Token::Type::function)
      std::cout << "function: " << ((TokenOperation*)token)->name << std::endl;
    tokens.pop();
  } 

  return 0;
}

