#include <cassert>
#include <iostream>
#include <random>
#include <vector>

#include "sort.h"

int main(void) {
  std::default_random_engine dre;
  std::vector<unsigned> vec;

  for (std::size_t i = 0; i < 1000; ++i) {
    vec.push_back(dre());
  }

  auto middle = vec.begin() + 500;
  std::sort(vec.begin(), middle);
  std::sort(middle, vec.end());
  InplaceMerge(vec.begin(), middle, vec.end());
  for (std::size_t i = 1; i < vec.size(); ++i) {
    assert(vec[i - 1] <= vec[i]);
  }

  std::shuffle(vec.begin(), vec.end(), std::default_random_engine(dre()));
  MergeSort(vec.begin(), vec.end());
  for (std::size_t i = 1; i < vec.size(); ++i) {
    assert(vec[i - 1] <= vec[i]);
  }

  std::shuffle(vec.begin(), vec.end(), std::default_random_engine(dre()));
  QuickSort(vec.begin(), vec.end());
  for (std::size_t i = 1; i < vec.size(); ++i) {
    assert(vec[i - 1] <= vec[i]);
  }

  unsigned value = vec[257];
  std::shuffle(vec.begin(), vec.end(), std::default_random_engine(dre()));
  assert(*Select(vec.begin(), vec.end(), 257) == value);
  for (std::size_t i = 0; i < vec.size(); ++i) {
    if (i < 257) {
      assert(vec[i] <= value);
    } else {
      assert(vec[i] >= value);
    }
  }

  std::shuffle(vec.begin(), vec.end(), std::default_random_engine(dre()));
  HeapSort(vec.begin(), vec.end());
  for (std::size_t i = 1; i < vec.size(); ++i) {
    assert(vec[i - 1] <= vec[i]);
  }

  return 0;
}