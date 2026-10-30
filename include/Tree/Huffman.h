#include "BinTree.h"
#include <cstring>
#include <fstream>
#include <list>

struct HuffChar;
using freq = unsigned int;
#define N_CHAR (0x7F - 0x20) // 可打印字符个数
using HuffTree = BinTree<HuffChar>;
using HuffForest = std::list<HuffTree *>;

struct HuffChar {
  char _c;
  freq _f;

  HuffChar(char c = '?', freq f = 0) : _c(c), _f(f) {}
  bool operator<(const HuffChar &other) const { return _f < other._f; }
  bool operator==(const HuffChar &other) const { return _f == other._f; }
};

inline int *stastic(char *file_name) {
  int *freqTable = new int[N_CHAR];
  memset(freqTable, 0, sizeof(int) * N_CHAR);
  std::ifstream fp(file_name);
  if(!fp){
    fp.close();
    return nullptr;
  }
  for(char ch; fp.get(ch);){
    if(ch >= 0x20 && ch <= 0x7F){
      freqTable[ch - 0x20]++;
    }
  }
  fp.close();
  return freqTable;
}