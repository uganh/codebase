#include <algorithm>

#include "heap.h"

template <typename Iterator>
void HeapSort(Iterator first, Iterator last) {
  using std::swap;
  BuildHeap(first, last);
  while (first < last) {
    swap(*first, *--last);
    Heapify(first, last, 0);
  }
}

template <typename Iterator>
void Reverse(Iterator first, Iterator last) {
  using std::swap;
  if (first < last) {
    while (first < --last) {
      swap(*first, *last);
      ++first;
    }
  }
}

/*
 * FIXME: O(n^2)0
 * */
template <typename Iterator>
void InplaceMerge(Iterator first, Iterator middle, Iterator last) {
  Iterator left  = first;
  Iterator right = middle;
  while (left < right && right < last) {
    /*
     * Loop invariants: [first...left) < [left...last)
     * */
    while (left < right && *left <= *right) {
      left++;
    }
    if (left == right) {
      break;
    }
    middle = right;
    while (right < last && *right < *left) {
      right++;
    }
    // Increasingly: [left...middle) > [middle...right)
    Reverse(left, middle);
    Reverse(middle, right);
    // Decreasingly: [left...middle) > [middle...right)
    // Decreasingly: [left...right)
    Reverse(left, right);
    left += right - middle;
  }
}

template <typename Iterator>
void MergeSort(Iterator first, Iterator last) {
  if (first + 1 < last) {
    Iterator middle = first + (last - first) / 2;
    MergeSort(first, middle);
    MergeSort(middle, last);
    InplaceMerge(first, middle, last);
  }
}

template <typename Iterator>
void QuickSort(Iterator first, Iterator last) {
  using std::swap;
  if (first + 1 < last) {
    /* TODO: Randomize */
    Iterator limit = last - 1;
    Iterator left = first, middle = first, right = first;
    while (right < limit) {
      /*
       * Loop invariants:
       *
       * (1) [first, left) < *limit
       * (2) [left, middle) = *limit
       * (3) [middle, limit) > *limit
       * */
      if (*right <= *limit) {
        swap(*middle, *right);
        if (*middle < *limit) {
          swap(*left++, *middle);
        }
        ++middle;
      }
      ++right;
    }
    swap(*middle++, *limit);
    QuickSort(first, left);
    QuickSort(middle, last);
  }
}

template <typename Iterator>
Iterator Select(Iterator first, Iterator last, size_t n_th) {
  using std::swap;
  
  if (last <= first && first + n_th >= last) {
    return last;
  }

  while (true) {
    /* TODO: Randomize */
    Iterator limit = last - 1;
    Iterator left = first, middle = first, right = first;
    while (right < limit) {
      /*
       * Loop invariants:
       *
       * (1) [first, left) < *limit
       * (2) [left, middle) = *limit
       * (3) [middle, limit) > *limit
       * */
      if (*right <= *limit) {
        swap(*middle, *right);
        if (*middle < *limit) {
          swap(*left++, *middle);
        }
        ++middle;
      }
      ++right;
    }
    swap(*middle++, *limit);
    if (first + n_th >= middle) {
      n_th -= (middle - first);
      first = middle;
    } else if (first + n_th >= left) {
      return first + n_th;
    } else {
      last = left;
    }
  }
}