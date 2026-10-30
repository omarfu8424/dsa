#include "BinTree.h"
#include <cstring>
#include <fstream>
#include <list>

using freq = unsigned int;
struct HuffChar {
  char _c;
  freq _f;

  HuffChar(char c = '\n', freq f = 0) : _c(c), _f(f) {}
  bool operator<(const HuffChar &other) const { return _f < other._f; }
  bool operator==(const HuffChar &other) const { return _f == other._f; }
};
#define N_CHAR (0x7F - 0x20 + 1) // 可打印字符个数
using HuffTree = BinTree<HuffChar>;
using HuffForest = std::list<HuffTree *>;

namespace Huffman {
inline int *stastic(const char *file_name) {
  int *freqTable = new int[N_CHAR];
  memset(freqTable, 0, sizeof(int) * N_CHAR);
  std::ifstream fp(file_name);
  if (!fp) {
    fp.close();
    return nullptr;
  }
  for (char ch = 0x20; fp.get(ch);) {
    if (ch >= 0x20 && ch <= 0x7F) {
      freqTable[ch - 0x20]++;
    }
  }
  fp.close();
  return freqTable;
}

inline HuffForest *initForest(int *freqTable) {
  auto *forest = new HuffForest;
  for (int i = 0; i < N_CHAR; ++i) {
    if (freqTable[i] == 0) {
      continue;
    }
    forest->push_back(new HuffTree);
    forest->back()->insertAsRoot(
        HuffChar(static_cast<char>(0x20 + i), freqTable[i]));
  }
  return forest;
}

inline HuffTree *getMinChar(HuffForest *forest) {
  if (forest == nullptr || forest->empty()) {
    return nullptr;
  }

  auto min = forest->begin();
  for (auto i = forest->begin(); i != forest->end(); ++i) {
    if ((*i)->root()->data() < (*min)->root()->data()) {
      min = i;
    }
  }

  auto *tree = *min;
  forest->erase(min);
  return tree;
}
inline HuffTree *generateHuffTree(HuffForest *forest) {
  while (forest->size() > 1) {
    auto *min1 = getMinChar(forest);
    auto *min2 = getMinChar(forest);
    auto *mintree = new HuffTree;
    mintree->insertAsRoot(
        HuffChar('\n', min1->root()->data()._f + min2->root()->data()._f));
    mintree->attachAsLc(mintree->root(), min1);
    mintree->attachAsRc(mintree->root(), min2);
    forest->push_back(mintree);
  }
  return forest->front();
}
inline void showCodeTable(){
}

} // namespace Huffman