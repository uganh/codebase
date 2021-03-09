#include <algorithm>
#include <cassert>
#include <cstddef>

template <typename Iterator>
void Heapify(Iterator first, Iterator last, size_t index) {
  if (first >= last) {
    return;
  }
  assert(index < last - first);

  while (true) {
    Iterator largest = first + index;
    Iterator left = first + 2 * index, right = left + 1;
    if (left<last && * left> * largest) {
      largest = left;
    }
    if (right<last && * right> * largest) {
      largest = right;
    }
    if (largest == first + index) {
      break;
    }
    using std::swap;
    std::swap(*largest, *(first + index));
    index = largest - first;
  }
}

template <typename Iterator>
void BuildHeap(Iterator first, Iterator last) {
  if (first < last) {
    ssize_t size = last - first;
    for (ssize_t index = size / 2; index >= 0; --index) {
      Heapify(first, last, index);
    }
  }
}
