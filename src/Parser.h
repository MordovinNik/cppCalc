#pragma once
#include <string>
#include <queue>
#include <iostream>
#include <stack>

class Token {
public:
  enum class Type {
    number,
    operation,
    function
  };

  virtual ~Token() = default;
  Type type;
};

class TokenNumber : public Token {
public:
  double value;
  TokenNumber() :value(0) { type = Type::number; }
};

class TokenOperation : public Token {
public:
  std::string name;
  TokenOperation() :name("") { type = Type::operation; }
};

class TokenFunction : public Token {
public:
  std::string name;
  TokenFunction() :name("") { type = Type::function; }
};

class Parser
{
public:
  Parser();
  std::queue<Token*>& Parse(const std::string& str);
  ~Parser();

private:
  std::string m_prepProcStr;
  std::queue<Token*> m_tokensQ;
  std::queue<Token*> m_rpnQ;

  std::string preProcess(const std::string& str);
  std::queue<Token*>& getTokens();
  std::queue<Token*>& toReversePolish();
  void getDigit(const std::string& str, int& index);
  void getFunction(const std::string& str, int& index);
  void clearQueue(std::queue<Token*>& queue);
  void clearStack(std::stack<Token*>& stack);
  int checkPriority(Token* token);
  void expandBrackets(TokenOperation* operation, std::stack<Token*>& opStack, std::queue<Token*>& m_rpnQ);
};