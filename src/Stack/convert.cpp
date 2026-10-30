#include <array>
#include <iostream>
#include <stack>
using namespace std;
namespace {
const array<char, 36> DIGIT{'0', '1', '2', '3', '4', '5', '6', '7', '8',
                            '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int char2int(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'A' && c <= 'Z') {
    return 10 + c - 'A';
  }
  return -1;
}
string convert(long long num, int now) {
  ; // from dec to now
  stack<char> stk;
  string res;
  while (num > 0) {
    stk.push(DIGIT.at(num % now));
    num /= now;
  }
  while (!stk.empty()) {
    res.push_back(stk.top());
    stk.pop();
  }
  return res;
}
string convert(const string &num, int pre, int now) {
  long long dec = 0;
  for (char c : num) {
    dec = (pre * dec) + char2int(c);
  }
  return convert(dec, now);
}

} // namespace

int main() {
  string num;
  cin >> num;
  int pre = 0;
  cin >> pre;
  int now = 0;
  cin >> now;
  string s = ::convert(num, pre, now);
  for (char i : s) {
    cout << i;
  }
  cout << '\n';
  return 0;
}