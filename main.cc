#include <iostream>
#include <type_traits>

template <size_t... V>
struct index_sequence : std::index_sequence<V...> {
  template <size_t T>
  static auto push_back_func() -> index_sequence<V..., T>;

  template <size_t T>
  using push_back_t = decltype(push_back_func<T>());
};

template <class Head, class... Tail>
struct print_index_sequence {
  static void print() {}
};

template <size_t Head, size_t... Tail>
struct print_index_sequence<index_sequence<Head, Tail...>> {
  static void print() {
    std::cout << Head << std::endl;
    print_index_sequence<index_sequence<Tail...>>::print();
  }
};

template <template <size_t N, size_t... P> class Func, int N,
          class IndexSequence>
struct apply_index_sequence {
  template <size_t... P>
  static auto apply(index_sequence<P...>) -> Func<N, P...>;
  using type = decltype(apply(std::declval<IndexSequence>()));
};

template <template <size_t N, size_t... P> class Func, int N,
          class IndexSequence>
using apply_index_sequence_t =
    apply_index_sequence<Func, N, IndexSequence>::type;

template <size_t N, size_t... P>
struct is_prime_number {
  template <size_t H, size_t... T>
  struct is_prime {
    static constexpr bool value =
        ((N < H * H) ? true : is_prime<T...>::value) && !(N % H == 0);
  };

  template <size_t T>
  struct is_prime<T> {
    static constexpr bool value = !(N % T == 0);
  };

  static constexpr bool value = is_prime<P...>::value;
};

template <size_t N>
struct prime_numbers {
  using search_type = prime_numbers<N * 19 / 20>::type;
  using prime_numbers_prev_t = prime_numbers<N - 1>::type;

  using type = std::conditional_t<
      apply_index_sequence_t<is_prime_number, N, prime_numbers_prev_t>::value,
      typename prime_numbers_prev_t::template push_back_t<N>,
      prime_numbers_prev_t>;
};

template <>
struct prime_numbers<2> {
  using type = index_sequence<2>;
};

int main() {
  static constexpr int N = 2000;
  print_index_sequence<prime_numbers<N>::type>::print();
  return 0;
}
