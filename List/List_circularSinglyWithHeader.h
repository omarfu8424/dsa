#pragma once
#include <functional>
template <typename T> struct PNode {
  T _value;
  PNode<T> *_link;
};
template <typename T, typename Compare = std::less<T>> class CSLH {
  PNode<T> *_header;
  Compare _comp;

public:
  CSLH() : _header(new PNode<T>), _comp() { _header->_link = _header; }
  CSLH<T, Compare> &operator=(const CSLH<T, Compare> &other) {
    if (&other == this) {
      return *this;
    }
    PNode<T> *p = _header->_link;
    while (p != _header) {
      PNode<T> *q = p;
      p = p->_link;
      delete q;
    }
    p = other._header->_link;
    while (p != other._header) {
      push_back(p->_value);
      p = p->_link;
    }
    return *this;
  }
  CSLH(const CSLH &other) : _header(new PNode<T>), _comp(other._comp) {
    _header->_link = _header;
    for (PNode<T> *p = other._header->_link; p != other._header; p = p->_link) {
      push_back(p->_value);
    }
  }
  ~CSLH() {
    PNode<T> *p = _header->_link;
    while (p != _header) {
      PNode<T> *q = p;
      p = p->_link;
      delete q;
    }
    delete _header;
  }

  class Iterator {
    PNode<T> *_ptr;
    friend CSLH<T, Compare>;

  public:
    Iterator(PNode<T> *ptr = nullptr) : _ptr(ptr) {}

    Iterator next() const { return _ptr->_link; }

    T &operator*() const { return _ptr->_value; }
    Iterator &operator++() {
      _ptr = _ptr->_link;
      return *this;
    }
    Iterator operator++(int) {
      Iterator temp = *this;
      _ptr = _ptr->_link;
      return temp;
    }
    bool operator!=(const Iterator &other) const {
      return this->_ptr != other._ptr;
    }
    bool operator==(const Iterator &other) const {
      return this->_ptr == other._ptr;
    }
  };
  //[begin, end)
  Iterator begin() { return Iterator(_header->_link); }
  Iterator end() { return Iterator(_header); }

  // read only
  [[nodiscard]] bool isEmpty() const { return _header->_link == _header; }

  // can write
  /*this function insert a input val that obide the order indicated by the comp.
  if your comp(a, b) is true when a<b, this function would insert val right
  behind the last node whose value is less than val. if your comp(a, b) is true
  when a>b, this function would insert val right behind the last node whose
  value is greater than val.
  we defaultly use std::less<T> as the comp, which means we make a ascending
  order list.
  */
  void insert(const T &val) {
    auto *node = new PNode<T>;
    node->_value = val;
    auto *pre = _header;
    auto *p = _header->_link;
    while (p != _header && _comp(p->_value, node->_value)) {
      pre = p;
      p = p->_link;
    }
    node->_link = p;
    pre->_link = node;
  }
  void push_back(const T &val) {
    auto *node = new PNode<T>;
    node->_value = val;
    auto *p = _header;
    while (p->_link != _header) {
      p = p->_link;
    }
    node->_link = _header;
    p->_link = node;
  }
  void erase(Iterator p) {
    if (p == end()) {
      return;
    }
    PNode<T> *ptr = _header;
    while (ptr->_link != p._ptr) {
      ptr = ptr->_link;
    }
    ptr->_link = p._ptr->_link;
    delete p._ptr;
  }
};