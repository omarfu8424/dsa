#include <List.h>
#include <iostream>
using namespace std;


namespace {
template <typename T> void test_sortSelect(List<T> test) {
  test.sort_select();
  cout << "after select-sort: ";
  test.show();
}

} // namespace


int main() {

  List<int> test;
  Rank n = 0;
  cin >> n;
  for (Rank i = 0; i < n; i++) {
    int x = 0;
    cin >> x;
    test.insert_as_last(x);
    cout << test[i] << " at " << &test[i] << '\n';
  }
  test_sortSelect(test);
  return 0;
}