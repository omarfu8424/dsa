#pragma once
#include "BinTree.h"
#include <cstring>
#include <fstream>
#include <list>
#include <ostream>
#include <string>

using freq = unsigned int;
struct HuffChar {
  char _c;
  freq _f;

  HuffChar(char c = '\n', freq f = 0) : _c(c), _f(f) {}
  bool operator<(const HuffChar &other) const { return _f < other._f; }
  bool operator==(const HuffChar &other) const { return _f == other._f; }
  
};

// 树结点的打印格式由数据类型自己决定, 而不是由 BinTree 猜:
// 叶子输出 "字符:频率", 内部结点输出 "[频率]".
// 内部结点用 '\n' 作哨兵, 因为可打印字符的取值范围 0x20~0x7F 不可能出现它.
inline std::ostream &operator<<(std::ostream &os, const HuffChar &c) {
  if (c._c == '\n') {
    return os << '[' << c._f << ']';
  }
  return os << c._c << ':' << c._f;
}
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
  if (forest == nullptr || forest->empty()) { // 空森林没有可取的树
    return nullptr;
  }
  while (forest->size() > 1) {
    auto *min1 = getMinChar(forest);
    auto *min2 = getMinChar(forest);
    auto *mintree = new HuffTree;
    mintree->insertAsRoot(
        HuffChar('\n', min1->root()->data()._f + min2->root()->data()._f));
    mintree->attachAsLSubTree(mintree->root(), min1);
    mintree->attachAsRSubTree(mintree->root(), min2);
    forest->push_back(mintree);
  }
  return forest->front();
}
namespace huffman_detail {
// 先序累积路径: 左分支记 0, 右分支记 1; 走到叶子, 路径就是该字符的码字.
inline void collectCodes(BinNodePosi<HuffChar> node, std::string &code,
                         std::ostream &os, long long &weightedLength,
                         long long &totalFrequency) {
  if (node == nullptr) {
    return;
  }
  if (node->isLeaf()) { // 码字前缀互不相同, 叶子不会出现在别的码字路径上
    const HuffChar &ch = node->data();
    const std::string bits = code.empty() ? std::string("0") : code;
    os << '\'' << ch._c << "': " << ch._f << " times -> " << bits << '\n';
    weightedLength += static_cast<long long>(bits.size()) * ch._f;
    totalFrequency += ch._f;
    return;
  }
  code.push_back('0');
  collectCodes(node->lc(), code, os, weightedLength, totalFrequency);
  code.back() = '1'; // 回溯到本结点后换右分支, 省一次 pop/push
  collectCodes(node->rc(), code, os, weightedLength, totalFrequency);
  code.pop_back();
}
} // namespace huffman_detail

// 打印哈夫曼编码表. 只有一种字符时树退化为单结点, 没有分支可走,
// 这里约定其码字为 "0"(空码字无法编码任何数据).
inline void showCodeTable(HuffTree *tree, std::ostream &os) {
  if (tree == nullptr || tree->isEmpty()) {
    os << "empty tree: no code table\n";
    return;
  }
  os << "code table (char: frequency -> codeword)\n";
  std::string code;
  long long weightedLength = 0;
  long long totalFrequency = 0;
  huffman_detail::collectCodes(tree->root(), code, os, weightedLength,
                               totalFrequency);
  if (totalFrequency > 0) {
    os << "average code length: "
       << static_cast<double>(weightedLength) /
              static_cast<double>(totalFrequency)
       << " bits/char\n";
  }
}

} // namespace Huffman