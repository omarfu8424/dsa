#include "Huffman.h"
#include <iomanip>
#include <iostream>
using namespace Huffman;
int main() {
  int *input = stastic("../Tree/input.txt");
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
  

  delete tree;
  delete forest;
  delete[] input;
  return 0;
}