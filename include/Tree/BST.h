#include "BinNode.h"
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
protected:
  BinNodePosi<T> _hot;

public:
  // 返回命中元素的引用, 若未命中,
  // 则返回nullptr(该nullptr是_hot的子节点,且是e应插入的位置).
  // 同时, _hot指向命中节点的父节点
  virtual BinNodePosi<T> &search(const T &e) {
    if (!this->root() || this->root()->data() == e) {
      _hot = nullptr;
      return this->rootRef();
    }
    _hot = this->root();
    for (;;) {
      BinNodePosi<T> &p = (_hot->data() < e) ? _hot->rcRef() : _hot->lcRef();
      if ((p == nullptr) || p->data() == e) {
        return p;
      }
      _hot = p;
    }
  }
  virtual BinNodePosi<T> insert(const T &e) {
    BinNodePosi<T>& p = search(e);
    if (p) {
      return nullptr;
    }
    p = new BinNode<T>(e, _hot);
    this->increaseSize();
    this->updateHeightAbove(p);
    return p;
  }
  // x是search传进来的
  // hot是x的父节点
  static BinNodePosi<T> removeAt(BinNodePosi<T> &x, BinNodePosi<T> &hot) {
    BinNodePosi<T> pRemove = x;
    BinNodePosi<T> succ = nullptr;
    if (!x->hasLChild()) {
      succ = x = x->rc(); // 无左子树, 直接挂右子树(可能存在)
    } else if (!x->hasRChild()) {
      succ = x = x->lc(); // 有左无右, 直接挂左子树
    } else {
      pRemove = x->succ();
      T temp = x->data();
      x->dataRef() = pRemove->data();
      pRemove->dataRef() = temp;
      BinNodePosi<T> u = pRemove->parent();
      (u == x) ? u->rcRef() : u->lcRef() = pRemove->rc();
      succ = pRemove->rc();
    }
    hot = pRemove->parent();
    if(succ) {
      succ->parentRef() = hot;
    }
    delete pRemove;
    return succ;
  }
  virtual bool remove(const T &e) {
    BinNodePosi<T> &p = search(e);
    if (!p) {
      return false;
    }
    removeAt(p, _hot);
    this->decreaseSize();
    this->updateHeightAbove(_hot);
    return true;
  }
};
