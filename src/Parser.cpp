#include "Parser.h"


Parser::Parser() {}

void Parser::clearQueue(std::queue<Token*>& queue)
{
  while (!queue.empty())
  {
    delete queue.front();
    queue.pop();
  }
}

void Parser::clearStack(std::stack<Token*>& stack)
{
  while (!stack.empty())
  {
    delete stack.top();
    stack.pop();
  }
}

Parser::~Parser()
{
  clearQueue(m_tokensQ);
  clearQueue(m_rpnQ);
}

std::string Parser::preProcess(const std::string& str)
{
  m_prepProcStr.clear();
  char last = 0;
  int bracketsCount = 0;
  for (auto& i : str)
  {
    switch (i)
    {
    case '(':
      if (isdigit(last))
      {
        m_prepProcStr += '*';
      }
      bracketsCount++;
      break;

    case ')':
      bracketsCount--;
      if (bracketsCount < 0)
      {
        throw std::exception("incorrect bracket sequence!\n");
      }
      break;

    case '-':
    case '+':
      if (last == 0 || isalpha(last) || last == '(' || last == '+'
          || last == '-' || last == '*' || last == '/' || last == ',' || last == '~')
      {
        if (i == '-')
        {
          last = '~';
          m_prepProcStr += last;
        }
        continue;
      }
      break;
    default:
      if (last == ')' && (isalpha(i) || isdigit(i)) || isdigit(last) && isalpha(i))
      {
        m_prepProcStr += '*';
      }
    }

    if (!isspace(i))
    {
      m_prepProcStr += i;
      last = i;
    }
  }
  if (bracketsCount != 0)
  {
    throw std::exception("incorrect bracket sequence!\n");
  }
  return m_prepProcStr;
}

std::queue<Token*>& Parser::getTokens()
{
  clearQueue(m_tokensQ);
  int i = 0;
  while (i < m_prepProcStr.length())
  {
    switch (m_prepProcStr[i])
    {
    case'+':
    case'-':
    case '*':
    case '/':
    case ',':
    case '(':
    case ')':
    case '~':
    {
      TokenOperation* operation = new TokenOperation();
      operation->name = m_prepProcStr[i];
      m_tokensQ.push(operation);
      i++;
      continue;
      break;
    }
    default:
      if (isdigit(m_prepProcStr[i]) || m_prepProcStr[i] == '.')
      {
        getDigit(m_prepProcStr, i);
        continue;
      }
      else if (isalpha(m_prepProcStr[i]))
      {
        getFunction(m_prepProcStr, i);
        continue;
      }
      throw std::exception("unknown symbol: " + m_prepProcStr[i]);
      break;
    }

  }
  return m_tokensQ;
}

void Parser::getDigit(const std::string& str, int& index)
{
  TokenNumber* number = new TokenNumber();
  size_t len;
  number->value = std::stod(str.substr(index), &len);
  m_tokensQ.push(number);
  index += len;
}

void Parser::getFunction(const std::string& str, int& index)
{
  int newInd = index;
  TokenFunction* function = new TokenFunction();

  while (newInd != str.length() && isalpha(str[newInd])) {
    function->name += str[newInd];
    newInd++;
  }
  index = newInd;
  m_tokensQ.push(function);
}

int Parser::checkPriority(Token* token)
{
  if (token->type == Token::Type::function)
    return 100;
  if (token->type == Token::Type::operation)
  {
    TokenOperation* operation = (TokenOperation*)token;
    switch (operation->name[0])
    {
    case '(':
      return 0;
    case ')':
      return 1;
    case ',':
      return 2;
    case '+':
    case '-':
      return 3;
    case '*':
    case '/':
      return 4;
    case '~':
      return 5;

    default:
      return 3;
      break;
    }
  }
}

void Parser::expandBrackets(TokenOperation* operation, std::stack<Token*>& opStack, std::queue<Token*>& m_rpnQ)
{
  //moving operations to result queue while openning bracket not found
  while (!opStack.empty() && ((TokenOperation*)opStack.top())->name != "(")
  {
    m_rpnQ.push(opStack.top());
    opStack.pop();
  }

  //no opening bracket found
  if (opStack.empty())
  {
    clearStack(opStack);
    if (operation->name == ",")
      throw std::exception("no openning bracket for binary function");
    else
      throw std::exception("no openning bracket");
  }

  //deleting openning bracket and moving function to result queue
  if (operation->name == ")")
  {
    opStack.pop();
    if (!opStack.empty() && opStack.top()->type == Token::Type::function)
    {
      m_rpnQ.push(opStack.top());
      opStack.pop();
    }
  }
}

std::queue<Token*>& Parser::toReversePolish()
{
  clearQueue(m_rpnQ);
  std::stack<Token*> opStack;

  while (!m_tokensQ.empty())
  {
    Token* tmp = m_tokensQ.front();
    m_tokensQ.pop();

    switch (tmp->type)
    {
    case Token::Type::number:
      m_rpnQ.push(tmp);
      if (!opStack.empty() && opStack.top()->type == Token::Type::function)
      {
        m_rpnQ.push(opStack.top());
        opStack.pop();
      }
      break;

    case Token::Type::function:
      opStack.push(tmp);
      break;

    case Token::Type::operation:
    {
      TokenOperation* operation = (TokenOperation*)tmp;

      if (operation->name == "," || operation->name == ")")
      {
        expandBrackets(operation, opStack, m_rpnQ);
      }
      else if (operation->name == "(")
      {
        opStack.push(operation);
      }
      else
      {
        int prior1 = checkPriority(operation);
        while (!opStack.empty() && prior1 <= checkPriority(opStack.top()))
        {
          m_rpnQ.push(opStack.top());
          opStack.pop();
        }
        opStack.push(operation);
      }
      break;
    }

    default:
      throw std::exception("something wrong with token's type");
      break;
    }
  }

  while (!opStack.empty())
  {
    m_rpnQ.push(opStack.top());
    opStack.pop();
  }

  return m_rpnQ;
}

std::queue<Token*>& Parser::Parse(const std::string& str)
{
  preProcess(str);
  getTokens();
  toReversePolish();
  return m_rpnQ;
}