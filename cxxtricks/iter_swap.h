#include <iterator>

/*
 * If the two iterators have the same value_type and real reference type, use
 * the fast implementation
 */
template <typename ForwardIterator1, typename ForwardIterator2>
constexpr bool use_swap = std::is_same<
    typename std::iterator_traits<ForwardIterator1>::value_type,
    typename std::iterator_traits<ForwardIterator2>::value_type>::value
    &&std::is_reference<
        typename std::iterator_traits<ForwardIterator1>::reference>::value
        &&std::is_reference<
            typename std::iterator_traits<ForwardIterator2>::reference>::value;

/*
 * Generic but could be inefficient
 */
template <typename ForwardIterator1, typename ForwardIterator2>
std::enable_if<!use_swap<ForwardIterator1, ForwardIterator2>>::type
iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2) {
  typename std::iterator_traits<ForwardIterator1>::value_type tmp = *i1;
  *i1 = *i2;
  *i2 = tmp;
}

/*
 * Fast
 */
template <typename ForwardIterator1, typename ForwardIterator2>
std::enable_if<use_swap<ForwardIterator1, ForwardIterator2>>::type
iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2) {
  std::swap(*iter1, *iter2);
}
