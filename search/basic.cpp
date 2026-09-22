#include "Fib.h"
#include <iomanip>
#include <iostream>
#include <vector>

using Rank = unsigned int;
using std::cout, std::cin, std::endl;
using std::setw;
using std::vector;

// 对已sort的序列，二分查找
template <typename T>
int search_bio(const vector<T> &arr, Rank lo, Rank hi, T x) {
  while (lo < hi) { // 查找区间为[lo, hi)
    Rank mi = (lo + hi) >> 1;
    if (x < arr[mi])
      hi = mi;
    else if (x > arr[mi])
      lo = mi + 1;
    else
      return mi;
  }
  return -1;
}

// Fib查找
template <typename T>
int search_fib(const vector<T> &arr, Rank lo, Rank hi, T x) {
  Fib f(hi - lo);
  Rank count = 0;
  while (lo < hi) {
    while (hi - lo < f.get())
      f.prev();
    Rank mi = lo + f.get() - 1;
    if (x < arr[mi]) {
      hi = mi;
    } else if (x > arr[mi]) {
      lo = mi + 1;
    } else {
      return count;
    }
  }
  return -1;
}
// 计算ASL
void search_fib_ASL(Rank size) {
  vector<int> arr(size);
  for (Rank i = 0; i < size; i++) {
    arr[i] = (static_cast<int>(i) * 2) + 1;
    cout << setw(8) << arr[i];
  }
  cout << endl;

  for (Rank i = 0; i < 2 * size + 1; i++) {
    bool find = false;
    Rank lo = 0, hi = size;
    Fib f(hi - lo);
    Rank count = 0;
    while (lo < hi) {
      while (hi - lo < f.get())
        f.prev();

      Rank mi = lo + f.get() - 1;

      if (i < arr[mi]) {
        hi = mi;
        count += 1;
      } else if (i > arr[mi]) {
        lo = mi + 1;
        count += 2;
      } else {
        find = true;
        count += 2;
        cout << setw(4) << count;
        break;
      }
    }
    if (!find)
      cout << setw(4) << count;
  }
}

int main() {
  vector<int> arr = {1, 3, 5, 7, 9, 11, 13};
  int x = 10;
  Rank index = search_fib(arr, 0, arr.size(), x);
  if (index != -1)
    cout << "Found " << x << " at index " << index << endl;
  else
    cout << x << " not found in the array." << endl;

  Rank size = 7;
  search_fib_ASL(size);
  cout << endl;
  return 0;
}