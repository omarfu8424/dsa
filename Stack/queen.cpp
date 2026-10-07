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
  if (N <= 0) {
    return;
  }
  vector<Queen> solu;
  Queen q(0, 0);
  do {
    if ((solu.size() >= N || q.y >= N)) { // (得到全局解 || 出界) 需回溯
      q = solu.back();                    // 回溯, 继续寻找
      solu.pop_back();
      q.y++;
    } else {
      while (q.y < N && find(solu, q)) { // 增加列数, 直到 (找到一个合理位置 || 出界)
        nCheck++;                        // 更新检查次数
        q.y++;                           // 检查下一个位置
      }
      if (q.y < N) {            // 检查合理位置是否存在
        solu.push_back(q);      // 合理解入栈
        if (solu.size() >= N) { // 检查是否放置了N个皇后, 即是否找到一个全局解
          nSolu++;
          // 打印全局解
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
        }
        q.x++;
        q.y = 0;
      }
    }

  } while ((0 < q.x) ||
           (q.y < N)); // 需要继续搜索: (当前皇后不在第一行, 可以回溯 || 当前皇后不出界, 可以向下一行搜索)
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