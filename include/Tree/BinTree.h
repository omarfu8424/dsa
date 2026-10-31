#pragma once
#include "BinNode.h"
#include <algorithm>

template <typename T> class BinTree {
private:
  Rank _size{};
  BinNodePosi<T> _root{};

protected:
  Rank stature(BinNodePosi<T> p) { return p ? p->height() : -1; }
  virtual Rank updateHeight(BinNodePosi<T> x) {
    return x->setHeight(1 + std::max(stature(x->lc()), stature(x->rc())));
  }
  virtual void updateHeightAbove(BinNodePosi<T> x) {
    while (x) {
      Rank ori = x->height();
      if (ori == updateHeight(x)) {
        break;
      }
      x = x->parent();
    }
  }

public:
  BinTree() : _root(nullptr) {}
  ~BinTree() {
    if (_size > 0) {
      remove(_root);
    }
  }
  // 深拷贝
  BinTree(const BinTree<T> &other) = delete;
  BinTree<T> &operator=(const BinTree<T> &other) = delete;

  // 状态判断
  [[nodiscard]] bool isEmpty() const { return _root == nullptr; }
  bool belongs(BinNodePosi<T> x) const {
    if (x == nullptr) {
      return false;
    }
    BinNodePosi<T> p = x;
    while (p->hasParent()) {
      p = p->parent();
    }
    return p == _root;
  }
  // 只读访问器
  [[nodiscard]] Rank size() const { return _size; }
  [[nodiscard]] const BinNodePosi<T> &root() const { return _root; }

  // 初始化
  BinNodePosi<T> insertAsRoot(const T &d) {
    if (!isEmpty()) {
      return nullptr;
    }
    _size = 1;
    return _root = new BinNode<T>(d);
  }

  // 插入孩子结点
  // assert: 该结点没有对应的孩子, 且该结点属于该树
  BinNodePosi<T> insertAsLC(BinNodePosi<T> x, const T &d) {
    if (x == nullptr || x->hasLChild() || !belongs(x)) {
      return nullptr;
    }
    BinNodePosi<T> child = x->insertLC(d);
    if (child == nullptr) {
      return nullptr;
    }
    updateHeightAbove(x);
    _size++;
    return child;
  }
  BinNodePosi<T> insertAsRC(BinNodePosi<T> x, const T &d) {
    if (x == nullptr || x->hasRChild() || !belongs(x)) {
      return nullptr;
    }
    BinNodePosi<T> child = x->insertRC(d);
    if (child == nullptr) {
      return nullptr;
    }
    updateHeightAbove(x);
    _size++;
    return child;
  }


  // 挂载子树
  // assert: 该结点没有对应的孩子, 且该结点属于该树, 且子树不为空, 且子树不属于该树
  BinNodePosi<T> attachAsLSubTree(BinNodePosi<T> x, BinTree<T> *&subTree) {
    if (x == nullptr || !belongs(x) || x->hasLChild() || subTree == nullptr ||
        subTree->isEmpty() || belongs(subTree->root()) ) {
      return nullptr;
    }
    x->setLC(subTree->root());
    x->lc()->setParent(x);
    _size += subTree->size();
    updateHeightAbove(x);
    subTree->_root = nullptr;
    subTree->_size = 0;
    delete subTree;
    subTree = nullptr;
    return x->lc();
  }

  BinNodePosi<T> attachAsRSubTree(BinNodePosi<T> x, BinTree<T> *&subTree) {
    if (x == nullptr || !belongs(x) || x->hasRChild() || subTree == nullptr ||
        subTree->isEmpty() || belongs(subTree->_root)) {
      return nullptr;
    }
    x->setRC(subTree->root());
    x->rc()->setParent(x);
    _size += subTree->size();
    updateHeightAbove(x);
    subTree->_root = nullptr;
    subTree->_size = 0;
    delete subTree;
    subTree = nullptr;
    return x->rc();
  }

  Rank remove(BinNodePosi<T> subRoot) {
    if (subRoot == nullptr) {
      return 0;
    }
    if (subRoot == _root) {
      _root = nullptr;
    } else if (subRoot->isRoot()) {
      return 0;
    } else if (subRoot->parent()->lc() == subRoot) {
      subRoot->parent()->setLC(nullptr);
    } else if (subRoot->parent()->rc() == subRoot) {
      subRoot->parent()->setRC(nullptr);
    } else {
      return 0;
    }
    updateHeightAbove(subRoot->parent());
    Rank n = removeAt(subRoot);
    _size -= n;
    return n;
  }
  Rank removeAt(BinNodePosi<T> p) {
    if (p == nullptr) {
      return 0;
    }
    Rank n = 1 + removeAt(p->rc()) + removeAt(p->lc());
    delete p;
    return n;
  }
};