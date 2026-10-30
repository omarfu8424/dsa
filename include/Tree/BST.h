#include "BinTree.h"
template <typename K, typename V> struct Entry {
  K key;
  V value;
  Entry(K k = K(), V v = V()) : key(k), value(v) {};
  Entry(const Entry<K, V> &other) : key(other.key), value(other.value) {};
  bool operator<(const Entry<K, V> &other) { return key < other.key; };
  bool operator>(const Entry<K, V> &other) { return key > other.key; };
  bool operator==(const Entry<K, V> &other) { return key == other.key; };
  bool operator!=(const Entry<K, V> &other) { return key != other.key; };
};
template <typename T> class BST : public BinTree<T> {
public:
  virtual BinNodePosi<T>& search(const T &e);
  virtual BinNodePosi<T> insert
};