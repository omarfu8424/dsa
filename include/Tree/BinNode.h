#pragma once
#include <cstdlib>
#include <queue>
#include <stack>
template <typename T> struct BinNode;
template <typename T> using BinNodePosi = BinNode<T> *;
using Rank = int;
// 遍历实现方式（可选参数用）
enum class TravImpl { Iteration, Recursion };

template <typename T> struct BinNode {
private:
  T _data;
  BinNodePosi<T> _parent;
  BinNodePosi<T> _lc;
  BinNodePosi<T> _rc;
  Rank _height{};

protected:
  // 先序遍历 root-l-r
  // 迭代
  template <typename VST>
  void travPre_iteration_1(BinNodePosi<T> root, VST &visit) {
    std::stack<BinNodePosi<T>> s;
    if (root) {
      s.push(root);
    }
    while (!s.empty()) {
      root = s.top();
      visit(root);
      s.pop();
      if (root->_rc) {
        s.push(root->_rc);
      }
      if (root->_lc) { // 这样左孩子先被visit
        s.push(root->_lc);
      }
    }
  }
  // template <typename VST>
  // void travPre_iteration_2(BinNodePosi<T> root, VST &visit);
  // 递归
  template <typename VST>
  void travPre_recursion(BinNodePosi<T> root, VST &visit) {
    if (!root) {
      return;
    }
    visit(root);
    travPre_recursion(root->_lc, visit);
    travPre_recursion(root->_rc, visit);
  }

  // 中序遍历 l-root-r
  // 迭代
  template <typename VST>
  void travIn_iteration(BinNodePosi<T> root, VST &visit) {
    std::stack<BinNodePosi<T>> s;
    BinNodePosi<T> curr = root;

    while (!s.empty() || curr) {
      while (curr) {
        s.push(curr);
        curr = curr->_lc;
      }
      // s.top()为当前未访问的最左侧结点
      curr = s.top();
      s.pop();
      visit(curr);
      curr = curr->_rc;
    }
  }
  // 递归
  template <typename VST>
  void travIn_recursion(BinNodePosi<T> root, VST &visit) {
    if (!root) {
      return;
    }
    travIn_recursion(root->_lc, visit);
    visit(root);
    travIn_recursion(root->_rc, visit);
  }

  // 后序遍历 l-r-root
  // 迭代
  void goToMostLeftLeaf(std::stack<BinNodePosi<T>> &
                            s) { // 迭代深入至最左侧leaf,
                                 // root至该leaf的通路上结点依次(先右后左地)入栈
    BinNodePosi<T> x = s.top();
    while (x) {
      if (x->_lc) {
        if (x->_rc) {
          s.push(x->_rc);
        }
        s.push(x->_lc);
      } else {
        s.push(x->_rc);
      }
      x = s.top();
    }
    s.pop();
  }
  template <typename VST>
  void travPost_iteration(BinNodePosi<T> root, VST &visit) {
    std::stack<BinNodePosi<T>> s;
    if (!root) {
      return;
    }
    BinNodePosi<T> x = root;
    s.push(x);
    while (!s.empty()) {
      if (s.top() != x->_parent) {
        goToMostLeftLeaf(s);
      }
      x = s.top();
      s.pop();
      visit(x);
    }
  }

  // 递归
  template <typename VST>
  void travPost_recursion(BinNodePosi<T> root, VST &visit) {
    if (!root) {
      return;
    }
    travPost_recursion(root->_lc, visit);
    travPost_recursion(root->_rc, visit);
    visit(root);
  }

public:
  // 构造
  BinNode() : _parent(nullptr), _lc(nullptr), _rc(nullptr) {};
  BinNode(T d, BinNodePosi<T> p = nullptr, BinNodePosi<T> l = nullptr,
          BinNodePosi<T> r = nullptr, Rank h = 0)
      : _data(d), _parent(p), _lc(l), _rc(r), _height(h) {};
  // 插入孩子
  BinNodePosi<T> insertLC(const T &d) { return _lc = new BinNode<T>(d, this); }
  BinNodePosi<T> insertRC(const T &d) { return _rc = new BinNode<T>(d, this); }
  // 只读访问器（常量引用/值，禁止外部直接改写指针）
  const T& data() const { return _data; }
  BinNodePosi<T> parent() const { return _parent; }
  BinNodePosi<T> lc() const { return _lc; }
  BinNodePosi<T> rc() const { return _rc; }
  Rank height() const { return _height; }
  // 写访问器（显式接口修改，便于维护结构一致性）
  BinNodePosi<T> setParent(BinNodePosi<T> p) { return _parent = p; }
  BinNodePosi<T> setLC(BinNodePosi<T> l) { return _lc = l; }
  BinNodePosi<T> setRC(BinNodePosi<T> r) { return _rc = r; }
  Rank setHeight(Rank h) {
    _height = h;
    return _height;
  }

  [[nodiscard]] Rank size() const {
    Rank s = 1;
    if (_lc) {
      s += _lc->size();
    }
    if (_rc) {
      s += _rc->size();
    }
    return s;
  }
  // 遍历
  template <typename VST> void travLevel(VST &visit); // 层次遍历
  template <typename VST>
  void travPre(VST &visit, TravImpl impl = TravImpl::Iteration); // 先序遍历
  template <typename VST>
  void travIn(VST &visit, TravImpl impl = TravImpl::Iteration);  // 中序遍历
  template <typename VST>
  void travPost(VST &visit, TravImpl impl = TravImpl::Iteration); // 后序遍历
};
// 层次遍历 BFS
template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST &visit) {
  std::queue<BinNodePosi<T>> q;
  if (this) {
    q.push(this);
  }
  while (!q.empty()) {
    BinNodePosi<T> root = q.front();
    visit(root);
    q.pop();
    if (root->_lc) {
      q.push(root->_lc);
    }
    if (root->_rc) {
      q.push(root->_rc);
    }
  }
}
// 先序遍历 PLR
template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST &visit, TravImpl impl) {
  switch (impl) {
  case TravImpl::Iteration:
    travPre_iteration_1(this, visit);
    break;
  case TravImpl::Recursion:
    travPre_recursion(this, visit);
    break;
  }
}
// 中序遍历 LPR
template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST &visit, TravImpl impl) {
  switch (impl) {
  case TravImpl::Iteration:
    travIn_iteration(this, visit);
    break;
  case TravImpl::Recursion:
    travIn_recursion(this, visit);
    break;
  }
}
// 后序遍历 LRP
template <typename T>
template <typename VST>
void BinNode<T>::travPost(VST &visit, TravImpl impl) {
  switch (impl) {
  case TravImpl::Iteration:
    travPost_iteration(this, visit);
    break;
  case TravImpl::Recursion:
    travPost_recursion(this, visit);
    break;
  }
}