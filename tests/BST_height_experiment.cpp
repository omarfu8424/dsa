#include "BST.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

namespace {

constexpr unsigned seed = 20261106;

int makeRandomTreeHeight(int n, std::mt19937 &generator) {
  std::vector<int> values(n);
  std::iota(values.begin(), values.end(), 1);
  std::shuffle(values.begin(), values.end(), generator);

  BST<int> tree;
  for (int value : values) {
    if (tree.insert(value) == nullptr) {
      throw std::runtime_error("random insertion unexpectedly rejected a key");
    }
  }
  return tree.root()->height();
}

void insertBalanced(BST<int> &tree, int first, int last) {
  if (first > last) {
    return;
  }
  const int middle = first + (last - first) / 2;
  if (tree.insert(middle) == nullptr) {
    throw std::runtime_error("balanced insertion unexpectedly rejected a key");
  }
  insertBalanced(tree, first, middle - 1);
  insertBalanced(tree, middle + 1, last);
}

int makeBalancedTreeHeight(int n) {
  BST<int> tree;
  insertBalanced(tree, 1, n);
  return tree.root()->height();
}

int makeSortedTreeHeight(int n) {
  BST<int> tree;
  for (int value = 1; value <= n; ++value) {
    if (tree.insert(value) == nullptr) {
      throw std::runtime_error("sorted insertion unexpectedly rejected a key");
    }
  }
  return tree.root()->height();
}

double averageRandomHeight(int n, int samples, std::mt19937 &generator) {
  double total = 0.0;
  for (int sample = 0; sample < samples; ++sample) {
    total += makeRandomTreeHeight(n, generator);
  }
  return total / samples;
}

} // namespace

int main() {
  constexpr int samples = 32;
  const std::vector<int> sizes{127, 511, 2047};
  std::mt19937 generator(seed);

  std::cout << "seed=" << seed << ", samples=" << samples << '\n';
  std::cout << "n,random_mean_height,random_mean_height_over_ln_n,"
               "sorted_height,balanced_height\n";

  for (int n : sizes) {
    const double randomHeight = averageRandomHeight(n, samples, generator);
    const int sortedHeight = makeSortedTreeHeight(n);
    const int balancedHeight = makeBalancedTreeHeight(n);
    const int expectedBalancedHeight =
        static_cast<int>(std::ceil(std::log2(n + 1))) - 1;

    if (sortedHeight != n - 1 || balancedHeight > expectedBalancedHeight) {
      std::cerr << "height invariant failed for n=" << n << '\n';
      return 1;
    }

    std::cout << n << ',' << std::fixed << std::setprecision(3)
              << randomHeight << ',' << randomHeight / std::log(n) << ','
              << sortedHeight << ',' << balancedHeight << '\n';
  }
}
