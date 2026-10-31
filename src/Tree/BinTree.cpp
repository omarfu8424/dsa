#include "BinTree.h"
#include <iostream>

int main() {
  BinTree<int> tree;
  auto *root = tree.insertAsRoot(10);
  auto *child = tree.insertAsRC(root, 5);

  auto *leftSubtree = new BinTree<int>;
  auto *leftRoot = leftSubtree->insertAsRoot(2);
  leftSubtree->insertAsLC(leftRoot, 1);
  leftSubtree->insertAsRC(leftRoot, 3);
  BinTree<int> *leftSubtreePointer = leftSubtree;
  tree.attachAsLSubTree(child, leftSubtreePointer);


  
  tree.root()->travLevel(binNode::PRINT_NODE);
  std::cout << '\n';
  return 0;
}