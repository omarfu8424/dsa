#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

namespace {
struct Queen {
  int x;
  int y;
  Queen(int xx = 0, int yy = 0) : x(xx), y(yy) {};
  bool operator==(Queen const &q) const {
    return (x + y == q.x + q.y) || (x - y == q.x - q.y) || (y == q.y);
  }
  bool operator!=(Queen const &q) const { return !(*this == q); }
};

bool find(const vector<Queen> &solu, const Queen &q, int flag = 0) {
  if (flag == 0) {
    return any_of(solu.begin(), solu.end(),
                  [&q](const Queen &i) { return i == q; });
  }
  return any_of(solu.begin(), solu.end(),
                [&q](const Queen &i) { return (i.x == q.x && i.y == q.y); });
}

void placeQueen(int N, int &nCheck, int &nSolu) {
  vector<Queen> solu;
  Queen q(0, 0);
  do {
    if ((solu.size() >= N || q.y >= N)) {
      if (solu.empty()) {
        break;
      }
      q = solu.back();
      solu.pop_back();
      q.y++;
    } else {
      while (q.y < N && find(solu, q)) { // 增加列数, 直到找到一个没有冲突的格子
        nCheck++;
        q.y++;
      }
      if (q.y < N) {            // 检查合理位置是否出界
        solu.push_back(q);      // 合理解入栈
        if (solu.size() >= N) { // 检查是否放置了N个皇后, 即是否找到一个全局解
          for (int i = 1; i < 2 * N; ++i) {
            cout << "-";
          }
          cout << '\n';
          for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
              if (find(solu, Queen(i, j), 1)) {
                cout << "Q ";
              } else {
                cout << ". ";
              }
            }
            cout << '\n';
          }
          nSolu++;
        }
        q.x++;
        q.y = 0;
      }
    }

  } while ((0 < q.x) || (q.y < N));
}

} // namespace

int main() {
  int N = 0;
  while (cin >> N) {
    int nCheck = 0;
    int nSolu = 0;
    placeQueen(N, nCheck, nSolu);
    cout << "\nN=" << N << " Check=" << nCheck << " Solu=" << nSolu << '\n';
  }
}