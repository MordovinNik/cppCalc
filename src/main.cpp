#include <iostream>
#include "Calculator.h"

int main() {
  Calculator calc;
  std::string line;
  double result;
  std::cout.precision(15);
  while (line != "quit")
  {
    std::getline(std::cin, line);
    try
    {
      result = calc.calculate(line);
      std::cout << "= " << result << std::endl;
    }
    catch (const std::exception& e)
    {
      std::cout<<"error: " << e.what() << std::endl;
    }
  }
}
