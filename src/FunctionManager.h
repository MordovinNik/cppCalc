#pragma once
#include <map>
#include <Windows.h>
#include <vector>
#include <string>
#include <stack>

typedef double(*UnaryFunc)(double);
typedef double(*BinaryFunc)(double, double);

struct externBinFunc{
  std::string name;
  BinaryFunc func;
};

struct externUnarFunc{
  std::string name;
  UnaryFunc func;
};

typedef std::vector<externBinFunc>&(*BinaryLoader)(void);
typedef std::vector<externUnarFunc>&(*UnaryLoader)(void);


class FunctionManager
{
public:
  FunctionManager();
  ~FunctionManager();

  double eval(const std::string& functionName, std::stack<double>& values);

private:
  std::map<std::string, UnaryFunc> m_unaryFunctions;
  std::map<std::string, BinaryFunc> m_binaryFuncrions;
  std::vector<HMODULE> Dlls;

  void readDlls(const std::string& Path = "./plugins/");
  void getFunctions(HMODULE dll);
  void initStdFunctions();
};