#pragma once
#include "BinNode.h"
#include "BinTree.h"
#include <random>
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
  // 按下面的方式组织3个结点与4棵子树, 使之成为一棵高度平衡的二叉搜索树
  // 返回新的子树根节点 b
  /***********
        b
      /   \
     a     c
    / \   / \
   T0 T1 T2 T3
  ***********/
  BinNodePosi<T> connect34(BinNodePosi<T> a, BinNodePosi<T> b, BinNodePosi<T> c,
                           BinNodePosi<T> T0, BinNodePosi<T> T1,
                           BinNodePosi<T> T2, BinNodePosi<T> T3) {
    a->lcRef() = T0;
    if (T0) {
      T0->parentRef() = a;
    }
    a->rcRef() = T1;
    if (T1) {
      T1->parentRef() = a;
    }
    this->updateHeight(a);
    c->lcRef() = T2;
    if (T2) {
      T2->parentRef() = c;
    }
    c->rcRef() = T3;
    if (T3) {
      T3->parentRef() = c;
    }
    this->updateHeight(c);
    b->lcRef() = a;
    a->parentRef() = b;
    b->rcRef() = c;
    c->parentRef() = b;
    this->updateHeight(b);
    return b; // 返回新的子树根节点
  }
  BinNodePosi<T> rotateAt(BinNodePosi<T> v) {
    BinNodePosi<T> p = v->parent();
    BinNodePosi<T> g = p->parent();
    if (p->isLChild()) {
      if (v->isLChild()) { // zig-zig
        p->parentRef() = g->parent();
        return connect34(v, p, g, v->lc(), v->rc(), p->rc(), g->rc());
      } else { // zig-zag
        v->parentRef() = g->parent();
        return connect34(p, v, g, p->lc(), v->lc(), v->rc(), g->rc());
      }
    } else {
      if (v->isRChild()) { // zag-zag
        p->parentRef() = g->parent();
        return connect34(g, p, v, g->lc(), p->lc(), v->lc(), v->rc());
      } else { // zag-zig
        v->parentRef() = g->parent();
        return connect34(g, v, p, g->lc(), v->lc(), v->rc(), p->rc());
      }
    }
  }

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
    BinNodePosi<T> &p = search(e);
    if (p) {
      return nullptr;
    }
    p = new BinNode<T>(e, _hot);
    this->increaseSize();
    this->updateHeightAbove(_hot);
    return p;
  }
  // x是search传进来的
  // hot是x的父节点
  static BinNodePosi<T> removeAtSwitchWithPredOrSucc(BinNodePosi<T> &x,
                                                     BinNodePosi<T> &hot) {
    BinNodePosi<T> pRemove = x;
    BinNodePosi<T> succ = nullptr;
    if (!x->hasLChild()) {
      succ = x = x->rc(); // 无左子树, 直接挂右子树(可能存在)
    } else if (!x->hasRChild()) {
      succ = x = x->lc(); // 有左无右, 直接挂左子树
    } else {
      static thread_local std::mt19937 generator(std::random_device{}());
      std::uniform_int_distribution<int> distribution(0, 1);
      pRemove = distribution(generator) == 0 ? x->succ() : x->pred();
      T temp = x->data();
      x->dataRef() = pRemove->data();
      pRemove->dataRef() = temp;
      BinNodePosi<T> u = pRemove->parent();

      succ = pRemove->hasLChild() // pRemove有左孩子 -> pRemove是pred,
                                  // 删掉后其唯一的孩子Lc要上来 反之,
                                  // pRemove是succ, 删掉后其唯一的孩子Rc要上来
                 ? pRemove->lc()
                 : pRemove->rc();
      (pRemove->isLChild() ? u->lcRef() : u->rcRef()) =
          succ; // 搭桥, 隔离pRemove
    }
    hot = pRemove->parent();
    if (succ) {
      succ->parentRef() = hot;
    }
    delete pRemove;
    return succ;
  }
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
      ((u == x) ? u->rcRef() : u->lcRef()) = succ = pRemove->rc();
    }
    hot = pRemove->parent();
    if (succ) {
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
