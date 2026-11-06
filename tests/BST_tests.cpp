#include "BST.h"

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

std::vector<int> inorder(const BST<int> &tree) {
  std::vector<int> values;
  auto visit = [&values](BinNodePosi<int> node) {
    values.push_back(node->data());
  };
  if (!tree.isEmpty()) {
    tree.root()->travIn(visit);
  }
  return values;
}

void testEmptyTree() {
  BST<int> tree;
  check(tree.isEmpty(), "a new BST must be empty");
  check(tree.size() == 0, "a new BST must have size zero");
  check(tree.root() == nullptr, "a new BST must have a null root");
  check(!tree.remove(10), "removing from an empty BST must fail");
}

void makeTree(BST<int> &tree) {
  for (int value : {4, 2, 6, 1, 3, 5, 7}) {
    check(tree.insert(value) != nullptr, "initial insertion must succeed");
  }
}

void testInsertSearchAndDuplicate() {
  BST<int> tree;
  makeTree(tree);
  check(tree.size() == 7, "size must count unique inserted values");
  check(tree.root()->data() == 4, "the first value must become the root");
  check(tree.search(5) != nullptr && tree.search(5)->data() == 5,
        "search must find an existing value");
  check(tree.search(9) == nullptr, "search must miss an absent value");
  check(tree.insert(4) == nullptr, "duplicate insertion must be rejected");
  check(tree.size() == 7, "duplicate insertion must not change size");
  check(inorder(tree) == std::vector<int>({1, 2, 3, 4, 5, 6, 7}),
        "inorder traversal must be sorted");
}

void testRemoveLeafOneChildAndTwoChildren() {
  BST<int> tree;
  makeTree(tree);

  check(tree.remove(1), "removing a leaf must succeed");
  check(tree.size() == 6, "leaf removal must decrease size");
  check(inorder(tree) == std::vector<int>({2, 3, 4, 5, 6, 7}),
        "leaf removal must preserve BST ordering");

  check(tree.remove(2), "removing a node with one child must succeed");
  check(tree.size() == 5, "one-child removal must decrease size");
  check(tree.root()->lc()->data() == 3, "the child must replace its parent");
  check(tree.root()->lc()->parent() == tree.root(),
        "replacement child must point to its new parent");

  check(tree.remove(4), "removing a node with two children must succeed");
  check(tree.size() == 4, "two-child removal must decrease size");
  check(tree.root()->data() == 3 || tree.root()->data() == 5,
        "two-child removal must replace the node with its predecessor or successor");
  check(inorder(tree) == std::vector<int>({3, 5, 6, 7}),
        "two-child removal must preserve BST ordering");
  check(!tree.remove(99), "removing an absent value must fail");
  check(tree.size() == 4, "failed removal must not change size");
}

using Test = std::pair<const char *, std::function<void()>>;

} // namespace

int main() {
  const std::vector<Test> tests{
      {"empty tree", testEmptyTree},
      {"insert, search, and duplicate", testInsertSearchAndDuplicate},
      {"remove leaf, one child, and two children",
       testRemoveLeafOneChildAndTwoChildren},
  };

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
