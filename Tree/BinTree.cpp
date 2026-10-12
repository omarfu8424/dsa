#include "BinTree.h"
#include <iostream>

int main() {
  BinTree<int> t;
  BinNodePosi<int> root = t.insertAsRoot(1);
  t.insertAsLC(root, 2);
  t.insertAsRC(root, 3);

  auto print = [](BinNodePosi<int> node) { std::cout << node->data() << ' '; };
  root->travIn(print);
  std::cout << '\n';
}