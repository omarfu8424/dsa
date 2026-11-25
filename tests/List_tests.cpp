#include "List.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

void check(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

std::vector<int> toVector(const List<int> &list) {
  std::vector<int> values;
  for (ListNodePosit<int> p = list.first(); list.valid(p); p = p->succ) {
    values.push_back(p->data);
  }
  return values;
}

void expectContents(const List<int> &list, std::vector<int> expected,
                    const std::string &message) {
  check(toVector(list) == expected, message);
}

List<int> makeList(std::vector<int> values) {
  List<int> list;
  for (int value : values) {
    list.insert_as_last(value);
  }
  return list;
}

void testEmptyList() {
  List<int> list;
  check(list.size() == 0, "a new list must have size zero");
  check(list.empty(), "a new list must be empty");
  check(!list.valid(list.first()), "first() must not be a valid node");
  check(!list.valid(nullptr), "nullptr must never be valid");
  check(toVector(list).empty(), "an empty list must have no elements");
}

void testInsert() {
  List<int> list;
  ListNodePosit<int> node = list.insert_as_first(1);
  check(node != nullptr && node->data == 1,
        "insert_as_first must return the new node");
  node = list.insert_as_last(3);
  check(node->data == 3, "insert_as_last must append");
  ListNodePosit<int> three = node;
  node = list.insert_after(4, three);
  check(node->data == 4, "insert_after must insert after the given node");
  node = list.insert_before(2, three);
  check(node->data == 2, "insert_before must insert before the given node");
  check(list.size() == 4, "size must grow with every insertion");
  expectContents(list, {1, 2, 3, 4}, "insertion must keep the expected order");
  list.show(); // 冒烟:人工目测输出 1 2 3 4
}

void testRemove() {
  List<int> list = makeList({1, 2, 3, 4, 5});
  check(list.remove(list.first()) == 1,
        "removing the head must return its data");
  check(list.remove(list.last()) == 5,
        "removing the tail must return its data");
  ListNodePosit<int> middle = list.first()->succ; // 2 的后继,值为 3
  check(list.remove(middle) == 3,
        "removing a middle node must return its data");
  expectContents(list, {2, 4}, "removal must preserve the order of the rest");
  check(list.size() == 2, "size must shrink with every removal");
}

void testFind() {
  List<int> list = makeList({3, 1, 2, 1});
  check(list.find(1) == list.last(),
        "whole-list find must return the match nearest the tail");
  check(list.find(9) == nullptr, "find must miss an absent value");
  ListNodePosit<int> third = list.first()->succ->succ; // 下标2,值为2
  check(list.find(1, 1, third) != nullptr && list.find(1, 1, third)->data == 1,
        "window [1,2) must contain 1");
  check(list.find(3, 1, third) == nullptr,
        "window [1,2) must not contain 3");
  check(list.find(3, 2, third) != nullptr && list.find(3, 2, third)->data == 3,
        "window [0,2) must contain 3");
}

void testSearch() {
  List<int> list = makeList({1, 3, 3, 5, 7});
  ListNodePosit<int> hit = list.search(3);
  check(hit != nullptr && hit->data == 3, "search(3) must find 3");
  check(hit == list.first()->succ->succ, "search(3) must return the last 3");
  check(list.search(4)->data == 3, "search(4) must return the largest <= 4");
  check(list.search(9)->data == 7, "search(9) must return the largest <= 9");
  hit = list.search(0);
  check(!list.valid(hit), "search(0) must return the header sentinel");
  check(hit->succ == list.first(), "the sentinel must precede the first node");
}

void testMax() {
  List<int> list = makeList({3, 7, 1, 5});
  check(list.max(list.first(), 4)->data == 7, "max over the whole list");
  check(list.max(list.first(), 2)->data == 7, "max over the first two");
  check(list.max(list.first()->succ->succ, 2)->data == 5,
        "max over the last two");
}

void testDedumlicate() {
  List<int> list = makeList({1, 2, 1, 3, 2});
  check(list.dedumlicate() == 2, "dedumlicate must report the removed count");
  expectContents(list, {1, 3, 2},
                 "dedumlicate keeps the last occurrence of each value, in "
                 "their original order");

  list = makeList({5, 5, 5, 5});
  check(list.dedumlicate() == 3, "dedumlicate must report all duplicates");
  expectContents(list, {5}, "dedumlicate must keep a single occurrence");

  list = makeList({1, 2, 3});
  check(list.dedumlicate() == 0,
        "dedumlicate must report zero for a unique list");
  list = makeList({7});
  check(list.dedumlicate() == 0, "dedumlicate must handle size < 2");
}

void testUniquify() {
  List<int> list = makeList({1, 1, 2, 2, 2, 3});
  check(list.uniquify() == 3, "uniquify must report the removed count");
  expectContents(list, {1, 2, 3}, "uniquify must keep one per run");

  list = makeList({5, 5, 5, 5});
  check(list.uniquify() == 3, "uniquify must report all duplicates");
  list = makeList({1, 2, 3});
  check(list.uniquify() == 0, "uniquify must report zero for a unique list");
  list = makeList({7});
  check(list.uniquify() == 0, "uniquify must handle size < 2");
}

void testSortInsert() {
  List<int> list = makeList({5, 3, 8, 1, 9, 2});
  list.sort_insert();
  expectContents(list, {1, 2, 3, 5, 8, 9},
                 "insertion sort must sort in ascending order");
  list = makeList({9, 8, 7, 6, 5});
  list.sort_insert();
  expectContents(list, {5, 6, 7, 8, 9},
                 "insertion sort must sort a reversed list");
  list = makeList({3, 1, 3, 2, 1});
  list.sort_insert();
  expectContents(list, {1, 1, 2, 3, 3},
                 "insertion sort must handle duplicates");
  list = makeList({4});
  list.sort_insert();
  expectContents(list, {4}, "insertion sort must handle a single element");
  List<int> empty;
  empty.sort_insert();
  check(empty.empty(), "insertion sort must handle an empty list");
}

void testSortSelect() {
  List<int> list = makeList({5, 3, 8, 1, 9, 2});
  list.sort_select();
  expectContents(list, {1, 2, 3, 5, 8, 9},
                 "selection sort must sort in ascending order");
  list = makeList({9, 8, 7, 6, 5});
  list.sort_select();
  expectContents(list, {5, 6, 7, 8, 9},
                 "selection sort must sort a reversed list");
  list = makeList({3, 1, 3, 2, 1});
  list.sort_select();
  expectContents(list, {1, 1, 2, 3, 3},
                 "selection sort must handle duplicates");
  list = makeList({4});
  list.sort_select();
  expectContents(list, {4}, "selection sort must handle a single element");
  List<int> empty;
  empty.sort_select();
  check(empty.empty(), "selection sort must handle an empty list");
}

void testSortMerge() {
  List<int> list = makeList({5, 3, 8, 1, 9, 2, 7}); // 奇数长度
  ListNodePosit<int> p = list.first();
  list.sort_merge(p, list.size());
  expectContents(list, {1, 2, 3, 5, 7, 8, 9},
                 "merge sort must sort an odd-length list");
  check(p == list.first(), "sort_merge must move p to the new head");

  list = makeList({5, 3, 8, 1, 9, 2}); // 偶数长度
  p = list.first();
  list.sort_merge(p, list.size());
  expectContents(list, {1, 2, 3, 5, 8, 9},
                 "merge sort must sort an even-length list");

  list = makeList({9, 8, 7, 6, 5});
  p = list.first();
  list.sort_merge(p, list.size());
  expectContents(list, {5, 6, 7, 8, 9},
                 "merge sort must sort a reversed list");

  list = makeList({3, 1, 3, 2, 1, 2});
  p = list.first();
  list.sort_merge(p, list.size());
  expectContents(list, {1, 1, 2, 2, 3, 3},
                 "merge sort must handle duplicates");

  list = makeList({1, 2, 3, 4, 5});
  p = list.first();
  list.sort_merge(p, list.size());
  expectContents(list, {1, 2, 3, 4, 5},
                 "merge sort must keep a sorted list unchanged");

  list = makeList({42});
  p = list.first();
  list.sort_merge(p, list.size());
  expectContents(list, {42}, "merge sort must handle a single element");

  List<int> empty;
  p = empty.first();
  empty.sort_merge(p, empty.size());
  check(empty.empty(), "merge sort must handle an empty list");
}

void testMergeTwoLists() {
  List<int> a = makeList({1, 3, 5});
  List<int> b = makeList({2, 4, 6});
  ListNodePosit<int> head = a.merge(a.first(), a.size(), b, b.first(), b.size());
  expectContents(a, {1, 2, 3, 4, 5, 6},
                 "merge must interleave two sorted lists into the first");
  check(b.size() == 0, "merge must move nodes out of the source list");
  check(head == a.first() && head->data == 1,
        "merge must return the head of the merged range");
}

void testCopyAndMove() {
  List<int> a = makeList({1, 2, 3});
  List<int> b(a);
  expectContents(b, {1, 2, 3}, "copy construction must copy all elements");
  a.insert_as_last(4);
  expectContents(b, {1, 2, 3}, "copy construction must be a deep copy");

  List<int> c;
  c = a;
  expectContents(c, {1, 2, 3, 4}, "copy assignment must copy all elements");
  a.insert_as_first(0);
  expectContents(c, {1, 2, 3, 4}, "copy assignment must be a deep copy");
  Rank sizeBefore = c.size();
  c = c; // 自赋值
  check(c.size() == sizeBefore, "self-assignment must not change size");
  expectContents(c, {1, 2, 3, 4}, "self-assignment must keep contents");

  List<int> d(std::move(c));
  expectContents(d, {1, 2, 3, 4}, "move construction must transfer elements");
  check(c.size() == 0 && c.empty(), "a moved-from list must be empty");
  c.insert_as_last(9); // 移动后源对象保留有效哨兵,可继续插入
  check(c.size() == 1, "a moved-from list must remain usable");
  expectContents(c, {9}, "a moved-from list must start empty");

  List<int> e;
  e = std::move(d);
  expectContents(e, {1, 2, 3, 4}, "move assignment must transfer elements");
  check(d.size() == 0, "the source of a move assignment must be empty");
  d.insert_as_first(8); // 移动赋值后源对象同样可复用
  expectContents(d, {8},
                 "the source of a move assignment must remain usable");

  List<int> f(a, 1, 2);
  expectContents(f, {1, 2}, "range construction must copy [r, r+n)");
  List<int> g(a.first()->succ, 2);
  expectContents(g, {1, 2}, "node-range construction must copy [r, r+n)");
}

void testDestructorSmoke() {
  {
    List<int> big;
    for (int i = 0; i < 10000; ++i) {
      big.insert_as_last(i);
    }
    check(big.size() == 10000, "bulk insertion must report the right size");
  } // 作用域退出时析构必须安全释放全部节点
}

using Test = std::pair<const char *, std::function<void()>>;

} // namespace

int main() {
  const std::vector<Test> tests{
      {"empty list", testEmptyList},
      {"insert first/last/before/after", testInsert},
      {"remove head/middle/tail", testRemove},
      {"find in unsorted range", testFind},
      {"search in sorted range", testSearch},
      {"max in range", testMax},
      {"dedumlicate", testDedumlicate},
      {"uniquify", testUniquify},
      {"insertion sort", testSortInsert},
      {"selection sort", testSortSelect},
      {"merge sort", testSortMerge},
      {"merge two sorted lists", testMergeTwoLists},
      {"copy and move semantics", testCopyAndMove},
      {"destructor smoke", testDestructorSmoke}};

  for (const auto &[name, test] : tests) {
    try {
      test();
      std::cout << "[PASS] " << name << '\n';
    } catch (const std::exception &error) {
      std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
      return 1;
    }
  }
  std::cout << tests.size() << " tests passed\n";
  return 0;
}
