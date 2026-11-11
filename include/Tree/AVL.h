#pragma once
#include "BST.h"
#include "BinNode.h"
#include <cstdlib>
template <typename T> class AVL : public BST<T> {
protected:
  Rank balancedFac(BinNodePosi<T> p) const { // 平衡因子
    return this->stature(p->lc()) - this->stature(p->rc());
  }
  bool balanced(BinNodePosi<T> p) const { return abs(balancedFac(p)) <= 1; }
  BinNodePosi<T> tallerChild(BinNodePosi<T> p) {
    if (this->stature(p->lc()) > this->stature(p->rc())) {
      return p->lcRef();
    }
    if (this->stature(p->lc()) < this->stature(p->rc())) {
      return p->rcRef();
    }
    return p->isLChild() ? p->lcRef() : p->rcRef();
  }

public:
  BinNodePosi<T> insert(const T &e) {
    BinNodePosi<T> x = BST<T>::insert(e);
    if (x == nullptr) {
      return nullptr;
    }
    for (BinNodePosi<T> g = this->_hot; g; g = g->parent()) {
      if (!balanced(g)) {
        BinNodePosi<T> &fromParentLink = this->fromParentTo(g);
        fromParentLink =
            this->rotateAt(this->tallerChild(this->tallerChild(g)));
        this->updateHeightAbove(fromParentLink->parent());
        break;
      } else {
        this->updateHeight(g);
      }
    }
    return x;
  }
  bool remove(const T &e) {
    BinNodePosi<T> &p = BST<T>::search(e);
    if (p == nullptr) {
      return false;
    }
    BST<T>::removeAt(p, this->_hot);
    this->decreaseSize();
    for (BinNodePosi<T> g = this->_hot; g;) {
      if (!balanced(g)) {
        BinNodePosi<T> &fromParentLink = this->fromParentTo(g);
        g = fromParentLink =
            this->rotateAt(this->tallerChild(this->tallerChild(g)));
      } else {
        this->updateHeight(g);
        g = g->parent();
      }
    }
    return true;
  }
};