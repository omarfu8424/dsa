#pragma once
#include "Graph.h"
#include <cassert>
#include <vector>

template <typename Tv, typename Te> class GraphMatrix : public Graph<Tv, Te> {
private:
  std::vector<Vertex<Tv>> _V;
  std::vector<std::vector<Edge<Te> *>> _E;
  using Graph<Tv, Te>::n;
  using Graph<Tv, Te>::e;

public:
  GraphMatrix() {
    n = 0;
    e = 0;
  }
  ~GraphMatrix() {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        delete _E[i][j];
      }
    }
  }
  GraphMatrix(const GraphMatrix &) = delete;
  GraphMatrix &operator=(const GraphMatrix &) = delete;
  GraphMatrix(GraphMatrix &&) = delete;
  GraphMatrix &operator=(GraphMatrix &&) = delete;

  // assert: vertex should exists.
  Vertex<Tv> &v(int i) override { return _V[i]; }
  // assert: edge should exists.
  Edge<Te> &edgeAt(int i, int j) override { return *_E[i][j]; }

  // edge
  int edgeCount(){return e;}
  bool exists(int i, int j) override {
    return (i >= 0 && i < n) && (j >= 0 && j < n) && (_E[i][j] != nullptr);
  }
  // i -> j
  void insert(Te const &edge, int w, int i, int j) override{
    if (exists(i, j)) {
      return;
    }
    _E[i][j] = new Edge<Te>(edge, w);
    e++;
    _V[i].outdegree++;
    _V[j].indegree++;
  }
  Te remove(int i, int j) override{
    assert(exists(i, j));
    Te backUp = edgeAt(i, j).data;
    delete _E[i][j];
    _E[i][j] = nullptr;
    e--;
    _V[i].outdegree--;
    _V[j].indegree--;
    return backUp;
  }

  // vertex
  int vertexCount(){return n;}
  // return new vertex's index
  int insert(Tv const &vertex) override {
    for (int r = 0; r < n; ++r) {
      _E[r].push_back(nullptr); // for each existing row, insert a new column
    }
    n++;
    // insert a new row
    _E.push_back(std::vector<Edge<Te> *>(n, (Edge<Te> *)nullptr));
    _V.push_back(Vertex<Tv>(vertex));
    return n - 1;
  }
  Tv remove(int i) override{
    assert(i >= 0 && i < n);
    Tv backUp = v(i).data;

    for (int j = 0; j < n; ++j) { // delete every edge begins at vertex[i]
      if (exists(i, j)) {
        remove(i, j);
      }
    }
    // delete every edge ends at vertex[i]
    for (int j = 0, k = 0; j < n; ++j, ++k) {
      if (j != i && exists(j, i)) {
        remove(j, i);
      }
      _E[k].erase(_E[k].begin() + i); // delete col[i]
    }
    _E.erase(_E.begin() + i); // delete row[i]
    n--;
    _V.erase(_V.begin() + i); // delete vertex[i]
    return backUp;
  }

  int firstNbr(int i) override{ return nextNbr(i, n); }
  int nextNbr(int i, int j) override{
    while (j > -1 && !exists(i, --j)) {
    }
    return j;
  }
};