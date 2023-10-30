#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

class Compiler {
 public:
  Compiler(std::string expression) : expression(expression), position(0) {}

  double parseExpression() {
    double value = parseTerm();
    while (expression[position] == '+' || expression[position] == '-') {
      char op = expression[position++];
      double term = parseTerm();
      if (op == '+') {
        value += term;
      } else {
        value -= term;
      }
    }
    return value;
  }

 private:
  std::string expression;
  size_t position;

  double parseTerm() {
    double value = parseFactor();
    while (expression[position] == '*' || expression[position] == '/') {
      char op = expression[position++];
      double factor = parseFactor();
      if (op == '*') {
        value *= factor;
      } else {
        if (factor != 0) {
          value /= factor;
        } else {
          std::cerr << "Ошибка: Деление на ноль!\n";
          std::exit(1);
        }
      }
    }
    return value;
  }

  double parseFactor() {
    if (std::isdigit(expression[position]) || expression[position] == '.') {
      std::stringstream ss;
      while (std::isdigit(expression[position]) ||
             expression[position] == '.') {
        ss << expression[position++];
      }
      double value;
      ss >> value;
      return value;
    } else if (expression[position] == '(') {
      position++;
      double value = parseExpression();
      if (expression[position] == ')') {
        position++;
      } else {
        std::cerr << "Ошибка: Ожидалась закрывающая скобка!\n";
        std::exit(1);
      }
      return value;
    } else {
      std::cerr << "Ошибка: Ожидалось число или открывающая скобка!\n";
      std::exit(1);
    }
  }
};

int main() {
  std::string expression;
  while (true) {
    std::cout << "Введите арифметическое выражение: ";
    std::getline(std::cin, expression);
    expression.erase(std::remove_if(expression.begin(), expression.end(),
                                    [](char c) { return std::isspace(c); }),
                     expression.end());
    if (expression.empty()) {
      break;
    }
    std::cout << "Вы ввели: " << expression << std::endl;
    Compiler compiler(expression);
    double result = compiler.parseExpression();

    std::cout << "Результат: " << result << std::endl;
  }
  return 0;
}