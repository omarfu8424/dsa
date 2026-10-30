#include "BinNode.h"
#include <algorithm>

#define stature(p) ((p) ? (p)->height() : -1)

template <typename T> class BinTree {
private:
  Rank _size{};
  BinNodePosi<T> _root;

protected:
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
    updateHeightAbove(x->insertLC(d));
    _size++;
    return x->lc();
  }
  BinNodePosi<T> insertAsRC(BinNodePosi<T> x, const T &d) {
    updateHeightAbove(x->insertRC(d));
    _size++;
    return x->rc();
  }

  BinNodePosi<T> attachAsLc(BinNodePosi<T> x, BinTree<T> *&subTree) {
    if (x->lc() != nullptr || subTree == nullptr) {
      return nullptr;
    }
    x->lc() = subTree->root();
    x->lc()->parent() = x;
    _size += subTree->size();
    updateHeightAbove(x);
    subTree->_root = nullptr;
    subTree->_size = 0;
    delete subTree;
    return x->lc();
  }

  BinNodePosi<T> attachAsRc(BinNodePosi<T> x, BinTree<T> *&subTree) {
    if (x->rc() != nullptr || subTree == nullptr) {
      return nullptr;
    }
    x->rc() = subTree->root();
    x->rc()->parent() = x;
    _size += subTree->size();
    updateHeightAbove(x);
    subTree->_root = nullptr;
    subTree->_size = 0;
    delete subTree;
    return x->rc();
  }

  Rank remove(BinNodePosi<T> subRoot) {
    if (subRoot == nullptr) {
      return 0;
    }
    if (subRoot == _root) {
      _root = nullptr;
    } else if (subRoot->parent()->lc() == subRoot) {
      subRoot->parent()->lc() = nullptr;
    } else if (subRoot->parent()->rc() == subRoot) {
      subRoot->parent()->rc() = nullptr;
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