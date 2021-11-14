#include "FunctionManager.h"
#include <string>
#include <iostream>

FunctionManager::FunctionManager()
{
  readDlls();

  initStdFunctions();
}

FunctionManager::~FunctionManager()
{
  for (auto& i : Dlls)
    FreeLibrary(i);
}

//toDo
void FunctionManager::readDlls(const std::string& path)
{
  WIN32_FIND_DATAA wfd;

  HANDLE hFile = FindFirstFileA((path + "*.dll").c_str(), &wfd);

  if (hFile == INVALID_HANDLE_VALUE)
    return;
  do
  {
    HMODULE hModule = LoadLibraryA((path + wfd.cFileName).c_str());
    if (hModule == NULL)
    {
      std::string err = "can't load library" + std::string(wfd.cFileName);
      throw std::exception(err.c_str());
    }

    BinaryLoader binLoader = (BinaryLoader)GetProcAddress(hModule, "sendBinary");
    if (binLoader)
    {
      std::vector<externBinFunc> binFunctions = binLoader();
      for (auto& i : binFunctions)
      {
        m_binaryFuncrions[i.name] = i.func;
        std::cout << "loaded binary func: "<<i.name <<std::endl;
      }
    }

    UnaryLoader unaryLoader = (UnaryLoader)GetProcAddress(hModule, "sendUnary");
    if (unaryLoader)
    {
      std::vector<externUnarFunc> unaryFunctions = unaryLoader();
      for (auto& i : unaryFunctions)
      {
        m_unaryFunctions[i.name] = i.func;
        std::cout << "loaded unary func: " << i.name << std::endl;
      }
    }
    Dlls.push_back(hModule);
  } while (FindNextFileA(hFile, &wfd) != NULL);
}

void FunctionManager::initStdFunctions()
{
  m_binaryFuncrions["+"] = [](double left, double right) {return left + right; };
  m_binaryFuncrions["-"] = [](double left, double right) {return left - right; };
  m_binaryFuncrions["*"] = [](double left, double right) {return left * right; };
  m_binaryFuncrions["/"] = [](double left, double right) {return left / right; };
  m_binaryFuncrions["pow"] = [](double left, double right) {return pow(left, right); };
  m_unaryFunctions["~"] = [](double val) {return -val; };
}

double FunctionManager::eval(const std::string& functionName, std::stack<double>& values)
{
  if (m_unaryFunctions.find(functionName) != m_unaryFunctions.end())
  {
    if (values.empty())
    {
      std::string err = "no arguments for function: " + functionName;
      throw std::exception(err.c_str());
    }

    double value = values.top();
    values.pop();
    double result = m_unaryFunctions[functionName](value);
    values.push(result);
    return result;
  }
  else if (m_binaryFuncrions.find(functionName) != m_binaryFuncrions.end())
  {
    if (values.size() < 2)
    {
      std::string err = "no arguments for function: " + functionName;
      throw std::exception(err.c_str());
    }
    double right = values.top();
    values.pop();
    double left = values.top();
    values.pop();
    double result = m_binaryFuncrions[functionName](left, right);
    values.push(result);
    return result;
  }

  std::string err = "no such function or operation: " + functionName;
  throw std::exception(err.c_str());
}