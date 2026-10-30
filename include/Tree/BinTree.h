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

  BinNodePosi<T> insertAsRoot(const T &d) {
    _size = 1;
    return _root = new BinNode<T>(d);
  }
  BinNodePosi<T> insertAsLC(BinNodePosi<T> x, const T &d) {
    BinNodePosi<T> child = x->insertLC(d);
    updateHeightAbove(x);
    _size++;
    return child;
  }
  BinNodePosi<T> insertAsRC(BinNodePosi<T> x, const T &d) {
    BinNodePosi<T> child = x->insertRC(d);
    updateHeightAbove(x);
    _size++;
    return child;
  }

  BinNodePosi<T> attachAsLc(BinNodePosi<T> x, BinTree<T> *&subTree) {
    if (x == nullptr || x->lc() != nullptr || subTree == nullptr ||
        subTree->root() == nullptr) {
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

  BinNodePosi<T> attachAsRc(BinNodePosi<T> x, BinTree<T> *&subTree) {
    if (x == nullptr || x->rc() != nullptr || subTree == nullptr ||
        subTree->root() == nullptr) {
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

  Rank size() { return _size; }
  bool empty() { return _root == nullptr; }
  BinNodePosi<T> &root() { return _root; }
};