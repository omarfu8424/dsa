#include "Huffman.h"
#include <iomanip>
#include <iostream>
using namespace Huffman;
int main() {
  int *input = stastic(DSA_INPUT_FILE);
  if (input == nullptr) {
    std::cerr << "failed to open the file\n";
    return 1;
  }
  for (int i = 0; i < N_CHAR; ++i) {
    if (input[i] > 0) {
      char ch = static_cast<char>(i + 0x20);
      std::cout << '\'' << ch << "': " << std::setw(4) << input[i]
                << " times\n";
    }
  }

  HuffForest *forest = initForest(input);
  HuffTree *tree = generateHuffTree(forest);
  if (tree == nullptr) { // 输入里没有可打印字符, 建不出树
    std::cerr << "no printable character in " DSA_INPUT_FILE "\n";
    delete forest;
    delete[] input;
    return 1;
  }

  std::cout << '\n';
  tree->printTree(); // 逆时针旋转 90 度看: 最上面是右子树
  std::cout << '\n';
  showCodeTable(tree, std::cout);

  delete tree;
  delete forest;
  delete[] input;
  return 0;
}