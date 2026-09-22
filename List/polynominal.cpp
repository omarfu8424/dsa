#include "List_circularSinglyWithHeader.h"
#include <cstdio>
#include <iostream>

using namespace std;

namespace {
struct Term {
  int _coef = 0;
  int _exp = 0;
  bool operator<(const Term &other) const { return _exp > other._exp; }
};
class Polynominal {
  CSLH<Term> _list;

public:
  Polynominal createPoly() {
    for (;;) {
      int coef = 0;
      cout << "coef:    ";
      cin >> coef;
      if (coef == 0) {
        break;
      }
      int exp = 0;
      cout << "exp:     ";
      cin >> exp;

      _list.insert({coef, exp});
    }
    return *this;
  }
  void uniquify() {
    auto pre = _list.begin();
    auto cur = pre.next();
    while (cur != _list.end()) {
      if ((*pre)._exp == (*cur)._exp) {
        (*pre)._coef += (*cur)._coef;
        _list.erase(cur);
        cur = pre.next();
        if ((*pre)._coef == 0) {
          _list.erase(pre);
          pre = cur;
          cur++;
        }
      } else {
        pre = cur;
        cur = cur.next();
      }
    }
  }
  void printPoly() {
    if (_list.isEmpty()) {
      cout << "p = 0\n";
      return;
    }
    auto p = _list.begin();
    printf("p = (%dx^%d)", (*p)._coef, (*p)._exp);
    p = p.next();
    for (; p != _list.end(); ++p) {
      printf("+(%dx^%d)", (*p)._coef, (*p)._exp);
    }
    cout << '\n';
  }
  Polynominal operator+(Polynominal &other) const {
    Polynominal res = *this;
    for (auto &p : other._list) {
      res._list.insert(p);
    }
    res.uniquify();
    return res;
  }
};

} // namespace

int main() {
  Polynominal p1; p1.createPoly();
  p1.printPoly();
  return 0;
}