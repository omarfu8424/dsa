#pragma once
#include <cstdint>

using VStatus = enum : std::uint8_t { UNDISCOVERED, DISCOVERED, VISITED };
using EType = enum : std::uint8_t {
  UNDETERMINED,
  TREE,
  CROSS,
  FORWARD,
  BACKWARD
};

template <typename Tv> struct Vertex {
  Tv data;
  int indegree, outdegree;
  VStatus status;
  int dTime, fTime;
  int parent, priority;
  Vertex(const Tv &d = Tv(0))
      : data(d), indegree(0), outdegree(0), status(UNDISCOVERED), dTime(-1),
        fTime(-1), parent(-1), priority(INT32_MAX) {}
};

template <typename Te> struct Edge {
  Te data;
  int weight;
  EType type;
  Edge(const Te &d, int w) : data(d), weight(w), type(UNDETERMINED) {}
};

template <typename Tv, typename Te> class Graph {
protected:
  void reset() {
    for (int i = 0; i < n; i++) {
      status(i) = UNDISCOVERED;
      dTime(i) = fTime(i) = -1;
      parent(i) = -1;
      priority(i) = INT32_MAX;
      for (int j = 0; j < n; j++) {
        if (exists(i, j)) {
          type(i, j) = UNDETERMINED;
        }
      }
    }
  }

public:
  virtual ~Graph() = default;
  int n; // 顶点总数
  int e; // 边总数

  // vertex
  Tv &vertex(int i) { return v(i).data; }
  VStatus &status(int i) { return v(i).status; }
  int &dTime(int i) { return v(i).dTime; }
  int &fTime(int i) { return v(i).fTime; }
  int &parent(int i) { return v(i).parent; }
  int &priority(int i) { return v(i).priority; }
  int inDegree(int i) { return v(i).indegree; }
  int outDegree(int i) { return v(i).outdegree; }

  // edge
  Te &edge(int i, int j) { return edgeAt(i, j).data; }
  int &weight(int i, int j) { return edgeAt(i, j).weight; }
  EType &type(int i, int j) { return edgeAt(i, j).type; }

  virtual Vertex<Tv> &v(int i) = 0;
  virtual Edge<Te> &edgeAt(int i, int j) = 0;

  virtual bool exists(int i, int j) = 0;
  virtual int insert(Tv const &vertex) = 0;
  virtual Tv remove(int i) = 0;
  virtual void insert(Te const &edge, int w, int i, int j) = 0;
  virtual Te remove(int i, int j) = 0;
  virtual int firstNbr(int i) = 0;
  virtual int nextNbr(int i, int j) = 0;
};