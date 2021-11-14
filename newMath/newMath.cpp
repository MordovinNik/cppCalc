#include <cmath>
#include "..//src/FunctionManager.h"

std::vector<externBinFunc> binary;
std::vector<externUnarFunc> unary;

extern "C" __declspec(dllexport) std::vector<externBinFunc>& sendBinary(void)
{
  binary.push_back({ "pow", [](double left,double right) {return pow(left,right); } });
  return binary;
}

extern "C" __declspec(dllexport) std::vector<externUnarFunc>& sendUnary(void)
{
  unary.push_back({ "abs",[](double a) {return (a > 0) ? a : -a; } });
  return unary;
}
