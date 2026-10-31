#include "BinTree.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void check(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

template <typename Traversal>
std::vector<int> collect(Traversal traversal) {
  std::vector<int> values;
  auto visit = [&values](BinNodePosi<int> node) {
    values.push_back(node->data());
  };
  traversal(visit);
  return values;
}

void testEmptyTree() {
  BinTree<int> tree;
  check(tree.isEmpty(), "a new tree must be empty");
  check(tree.size() == 0, "a new tree must have size zero");
  check(tree.root() == nullptr, "a new tree must have a null root");
  check(tree.remove(nullptr) == 0, "removing null must remove nothing");
}

void testInsertionLinksAndHeights() {
  BinTree<int> tree;
  auto root = tree.insertAsRoot(4);
  auto left = tree.insertAsLC(root, 2);
  auto right = tree.insertAsRC(root, 6);
  auto leftLeft = tree.insertAsLC(left, 1);
  auto leftRight = tree.insertAsRC(left, 3);
  auto rightRight = tree.insertAsRC(right, 7);

  check(!tree.isEmpty(), "inserted tree must not be empty");
  check(tree.size() == 6, "size must count every inserted node");
  check(root == tree.root(), "insertAsRoot must return the tree root");
  check(root->parent() == nullptr, "root must not have a parent");
  check(root->lc() == left && root->rc() == right,
        "root child links must be maintained");
  check(left->parent() == root && right->parent() == root,
        "child parent links must be maintained");
  check(leftLeft->parent() == left && leftRight->parent() == left &&
            rightRight->parent() == right,
        "grandchild parent links must be maintained");
  check(root->size() == 6, "node size must count the complete subtree");
  check(left->height() == 1 && right->height() == 1,
        "leaf-parent heights must be updated");
  check(root->height() == 2, "root height must be updated");
}

void testTraversals() {
  BinTree<int> tree;
  auto root = tree.insertAsRoot(4);
  auto left = tree.insertAsLC(root, 2);
  auto right = tree.insertAsRC(root, 6);
  tree.insertAsLC(left, 1);
  tree.insertAsRC(left, 3);
  tree.insertAsLC(right, 5);
  tree.insertAsRC(right, 7);

  const std::vector<int> pre{4, 2, 1, 3, 6, 5, 7};
  const std::vector<int> in{1, 2, 3, 4, 5, 6, 7};
  const std::vector<int> post{1, 3, 2, 5, 7, 6, 4};
  const std::vector<int> level{4, 2, 6, 1, 3, 5, 7};

  check(collect([&](auto &visit) { root->travPre(visit); }) == pre,
        "iterative preorder traversal is incorrect");
  check(collect([&](auto &visit) {
          root->travPre(visit, TravImpl::Recursion);
        }) == pre,
        "recursive preorder traversal is incorrect");
  check(collect([&](auto &visit) { root->travIn(visit); }) == in,
        "iterative inorder traversal is incorrect");
  check(collect([&](auto &visit) {
          root->travIn(visit, TravImpl::Recursion);
        }) == in,
        "recursive inorder traversal is incorrect");
  check(collect([&](auto &visit) { root->travPost(visit); }) == post,
        "iterative postorder traversal is incorrect");
  check(collect([&](auto &visit) {
          root->travPost(visit, TravImpl::Recursion);
        }) == post,
        "recursive postorder traversal is incorrect");
  check(collect([&](auto &visit) { root->travLevel(visit); }) == level,
        "level-order traversal is incorrect");
}

void testSingleNodeTraversals() {
  BinTree<int> tree;
  auto root = tree.insertAsRoot(42);
  const std::vector<int> expected{42};

  check(collect([&](auto &visit) { root->travPre(visit); }) == expected,
        "single-node preorder traversal is incorrect");
  check(collect([&](auto &visit) { root->travIn(visit); }) == expected,
        "single-node inorder traversal is incorrect");
  check(collect([&](auto &visit) { root->travPost(visit); }) == expected,
        "single-node postorder traversal is incorrect");
  check(collect([&](auto &visit) { root->travLevel(visit); }) == expected,
        "single-node level traversal is incorrect");
}

void testAttachAndRejectInvalidAttach() {
  BinTree<int> tree;
  auto root = tree.insertAsRoot(10);
  auto child = tree.insertAsLC(root, 5);

  auto *leftSubtree = new BinTree<int>;
  auto leftRoot = leftSubtree->insertAsRoot(2);
  leftSubtree->insertAsLC(leftRoot, 1);
  leftSubtree->insertAsRC(leftRoot, 3);
  BinTree<int> *leftSubtreePointer = leftSubtree;
  check(tree.attachAsLSubTree(child, leftSubtreePointer) == leftRoot,
        "left subtree must be attached");
  check(leftSubtreePointer == nullptr,
        "attached subtree pointer must be consumed");
  check(tree.size() == 5, "tree size must include attached nodes");
  check(tree.root()->height() == 3, "height must include attached nodes");
  check(leftRoot->parent() == child, "attached root parent must be updated");

  BinTree<int> emptySubtree;
  BinTree<int> *emptyPointer = &emptySubtree;
  check(tree.attachAsRSubTree(child, emptyPointer) == nullptr,
        "an empty subtree must not be attached");
  check(emptyPointer == &emptySubtree,
        "rejected subtree ownership must remain with the caller");

  BinTree<int> occupiedSubtree;
  occupiedSubtree.insertAsRoot(99);
  BinTree<int> *occupiedPointer = &occupiedSubtree;
  check(tree.attachAsLSubTree(child, occupiedPointer) == nullptr,
        "an occupied child position must reject attachment");
  check(occupiedPointer == &occupiedSubtree,
        "rejected occupied attachment must preserve ownership");
  check(tree.size() == 5, "rejected attachment must not change size");
}

void testRemove() {
  BinTree<int> tree;
  auto root = tree.insertAsRoot(4);
  auto left = tree.insertAsLC(root, 2);
  tree.insertAsLC(left, 1);
  tree.insertAsRC(left, 3);
  auto right = tree.insertAsRC(root, 6);
  tree.insertAsRC(right, 7);

  check(tree.remove(left) == 3, "remove must return removed subtree size");
  check(tree.size() == 3, "remove must update tree size");
  check(root->lc() == nullptr, "removed subtree must be detached");
  check(root->height() == 2, "remove must update ancestor heights");
  check(collect([&](auto &visit) { root->travIn(visit); }) ==
            std::vector<int>({4, 6, 7}),
        "remaining tree is incorrect after removing a subtree");

  check(tree.remove(nullptr) == 0, "removing null after mutation must be safe");
  check(tree.remove(root) == 3, "removing the root must remove all nodes");
  check(tree.isEmpty() && tree.size() == 0 && tree.root() == nullptr,
        "removing the root must empty the tree");
}

using Test = std::pair<const char *, std::function<void()>>;

} // namespace

int main() {
  const std::vector<Test> tests{
      {"empty tree", testEmptyTree},
      {"insertion links and heights", testInsertionLinksAndHeights},
      {"all traversals", testTraversals},
      {"single-node traversals", testSingleNodeTraversals},
      {"attach and invalid attach", testAttachAndRejectInvalidAttach},
      {"remove", testRemove},
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
