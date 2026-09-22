#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool isMatch(const string &s) {
  stack<char> stk;
  for (char i : s) {
    switch (i) {
    case '(':
    case '{':
    case '[':
      stk.push(i);
      break;
    case ')':
      if (stk.empty() || stk.top() != '(') {
        return false;
      }
      stk.pop();
      break;
    case '}':
      if (stk.empty() || stk.top() != '{') {
        return false;
      }
      stk.pop();
      break;
    case ']':
      if (stk.empty() || stk.top() != '[') {
        return false;
      }
      stk.pop();
      break;

    default:
      break;
    }
  }
  return stk.empty();
}

int main() {
  string str;
  cin >> str;
  if (isMatch(str)) {
    cout << "valid expression" << '\n';
  } else {
    cout << "invalid" << '\n';
  }
  return 0;
}