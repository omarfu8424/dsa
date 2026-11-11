#include "AVL.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
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

int validateNode(BinNodePosi<int> node, BinNodePosi<int> parent, int *count,
                 int *minimum, int *maximum) {
  if (node == nullptr) {
    *minimum = 0;
    *maximum = 0;
    return -1;
  }

  check(node->parent() == parent, "every parent pointer must be reciprocal");
  int leftMinimum = 0;
  int leftMaximum = 0;
  int rightMinimum = 0;
  int rightMaximum = 0;
  const int leftHeight =
      validateNode(node->lc(), node, count, &leftMinimum, &leftMaximum);
  const int rightHeight =
      validateNode(node->rc(), node, count, &rightMinimum, &rightMaximum);
  if (node->lc()) {
    check(leftMaximum < node->data(), "left subtree must contain smaller keys");
  }
  if (node->rc()) {
    check(rightMinimum > node->data(), "right subtree must contain larger keys");
  }
  check(std::abs(leftHeight - rightHeight) <= 1,
        "every node must satisfy the AVL balance invariant");
  check(node->height() == 1 + std::max(leftHeight, rightHeight),
        "stored height is incorrect at node " + std::to_string(node->data()));

  ++*count;
  *minimum = node->lc() ? leftMinimum : node->data();
  *maximum = node->rc() ? rightMaximum : node->data();
  return node->height();
}

void validateTree(const AVL<int> &tree, int expectedSize) {
  check(tree.size() == expectedSize, "tree size is incorrect");
  if (expectedSize == 0) {
    check(tree.root() == nullptr, "an empty tree must have no root");
    return;
  }

  int count = 0;
  int minimum = 0;
  int maximum = 0;
  validateNode(tree.root(), nullptr, &count, &minimum, &maximum);
  check(count == expectedSize, "tree size must match reachable nodes");
}

std::vector<int> inorder(const AVL<int> &tree) {
  std::vector<int> values;
  if (!tree.isEmpty()) {
    auto visit = [&values](BinNodePosi<int> node) {
      values.push_back(node->data());
    };
    tree.root()->travIn(visit);
  }
  return values;
}

void testEmptyTree() {
  AVL<int> tree;
  validateTree(tree, 0);
  check(!tree.remove(1), "removing from an empty AVL must fail");
  check(tree.insert(10) != nullptr, "first insertion must succeed");
  validateTree(tree, 1);
}

void testAllInsertionRotations() {
  const std::vector<std::vector<int>> rotations{
      {3, 2, 1}, {1, 2, 3}, {3, 1, 2}, {1, 3, 2}};
  for (const std::vector<int> &values : rotations) {
    AVL<int> tree;
    for (int value : values) {
      check(tree.insert(value) != nullptr, "unique insertion must succeed");
    }
    validateTree(tree, 3);
    check(tree.root()->data() == 2, "each insertion rotation must center 2");
    check(tree.root()->lc()->data() == 1 &&
              tree.root()->rc()->data() == 3,
          "rotation must preserve the expected children");
  }
}

void testSearchDuplicateAndOrdering() {
  AVL<int> tree;
  for (int value : {4, 2, 6, 1, 3, 5, 7}) {
    check(tree.insert(value) != nullptr, "initial insertion must succeed");
  }
  check(tree.insert(4) == nullptr, "duplicate insertion must be rejected");
  check(tree.search(5) != nullptr && tree.search(5)->data() == 5,
        "search must find existing values");
  check(tree.search(99) == nullptr, "search must miss absent values");
  check(inorder(tree) == std::vector<int>({1, 2, 3, 4, 5, 6, 7}),
        "inorder traversal must remain sorted");
  validateTree(tree, 7);
}

void testRemovalAndRebalancing() {
  AVL<int> tree;
  for (int value = 1; value <= 100; ++value) {
    check(tree.insert(value) != nullptr, "sorted insertion must succeed");
    try {
      validateTree(tree, value);
    } catch (const std::exception &error) {
      throw std::runtime_error("after inserting " + std::to_string(value) +
                               ": " + error.what());
    }
  }

  std::vector<int> removalOrder;
  for (int value = 1; value <= 100; ++value) {
    removalOrder.push_back(value);
  }
  std::mt19937 generator(20261111);
  std::shuffle(removalOrder.begin(), removalOrder.end(), generator);
  int expectedSize = 100;
  for (int value : removalOrder) {
    check(tree.remove(value), "existing value removal must succeed");
    --expectedSize;
    try {
      validateTree(tree, expectedSize);
    } catch (const std::exception &error) {
      throw std::runtime_error("after removing " + std::to_string(value) +
                               ": " + error.what());
    }
  }
  check(!tree.remove(1), "removing an absent value must fail");
}

using Test = std::pair<const char *, std::function<void()>>;

} // namespace

int main() {
  const std::vector<Test> tests{{"empty tree", testEmptyTree},
                                {"all insertion rotations",
                                 testAllInsertionRotations},
                                {"search, duplicate, and ordering",
                                 testSearchDuplicateAndOrdering},
                                {"removal and rebalancing",
                                 testRemovalAndRebalancing}};
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
}