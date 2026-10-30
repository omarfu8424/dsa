#include <functional>
#include <iostream>
#include <stack>
#include <unordered_map>
using namespace std;

namespace {
const unordered_map<char, function<int(int, int)>> K_OPERATIONS = {
    {'+', [](int a, int b) { return a + b; }},
    {'-', [](int a, int b) { return a - b; }},
    {'*', [](int a, int b) { return a * b; }},
    {'/', [](int a, int b) { return a / b; }}};

int rpToRes(stack<int> &stk, const string &s) {
  for (char c : s) {
    if (K_OPERATIONS.find(c) != K_OPERATIONS.end()) {
      int b = stk.top();
      stk.pop();
      int a = stk.top();
      stk.pop();
      stk.push(K_OPERATIONS.at(c)(a, b));
    } else {
      stk.push(c - '0');
    }
  }
  return stk.top();
}

} // namespace

int main() {
  stack<int> stk;
  string rp;
  char c = 0;
  int res = 0;
  while (cin >> c) {
    if (c == '#') {
      break;
    }
    rp.push_back(c);
  }
  res = rpToRes(stk, rp);
  cout << res << '\n';
}