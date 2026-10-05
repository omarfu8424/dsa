#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;
namespace {
/**************************
isp: in-stack priority
icp: incoming priority

icp > isp  push
icp < isp  pop and caculate
**************************/
struct Prio {
  int isp;
  int icp;
};

const unordered_map<char, Prio> PRIO = {
    {'+', {3, 4}}, {'-', {3, 4}}, {'*', {5, 6}}, {'/', {5, 6}}, {'^', {7, 8}},
    {'!', {8, 9}}, {'(', {1, 9}}, {')', {9, 2}}, {'\0', {0, 0}}};

// get valid number from a expression
void readNumber(char *&p, stack<double> &o) {
  double num = 0.0;
  while (isdigit(static_cast<unsigned char>(*p)) == 0) {
    p++;
  }
  while (isdigit(static_cast<unsigned char>(*p)) != 0) {
    num = (num * 10) + (*p - '0');
    p++;
  }
  if (*p == '.') {
    p++;
    double fraction = 1.0;
    while (isdigit(static_cast<unsigned char>(*p)) != 0) {
      num += (*p - '0') * (fraction /= 10);
      p++;
    }
  }
  o.push(num);
}
double calcu(double num, char op) {
  double res = 0.0;
  switch (op) {
  case '!':
    res = tgamma(num + 1);
    break;
  default:
    exit(1);
  }
  return res;
}
double calcu(double opnd1, double opnd2, char op) {
  double res = 0.0;
  switch (op) {
  case '+':
    res = opnd1 + opnd2;
    break;
  case '-':
    res = opnd1 - opnd2;
    break;
  case '*':
    res = opnd1 * opnd2;
    break;
  case '/':
    res = opnd1 / opnd2;
    break;
  case '^':
    res = pow(opnd1, opnd2);
    break;
  default:
    exit(1);
  }
  return res;
}

double evaluate(char *&p, stack<string> &rpn) {
  stack<char> optr;   // symbol
  stack<double> opnd; // number
  optr.push('\0');
  while (!optr.empty()) {
    if (isdigit(static_cast<unsigned char>(*p)) !=
        0) { // following number should be push to the opnd stack
      readNumber(p, opnd);
      rpn.push(to_string(opnd.top()));
    } else {
      int diff = PRIO.at(optr.top()).icp - PRIO.at(*p).isp;
      int sign = static_cast<int>(diff > 0) - static_cast<int>(diff < 0);
      switch (sign) {
      case 1: // icp > isp: push
        optr.push(*p);
        p++;
        break;
      case 0: // icp = isp: ')' meets '(' || '\0' meets '\0'
        optr.pop();
        p++;
        break;
      case -1: // icp < isp: pop and calculate
        char op = optr.top();
        rpn.push(to_string(op));
        if (op == '!') {
          opnd.push(calcu(opnd.top(), op));
        } else {
          double opnd1 = opnd.top();
          opnd.pop();
          double opnd2 = opnd.top();
          opnd.pop();
          opnd.push(calcu(opnd1, opnd2, op));
        }
        break;
      }
    }
  }
  return opnd.top();
}
} // namespace

int main() {
  // test readNumber
  char a[] = "(2+3)*5";
  char *p = a;
  stack<string> rpn;
  evaluate(p, rpn);
  return 0;
}