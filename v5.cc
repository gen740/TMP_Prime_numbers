#include <iostream>

template <size_t... V>
struct index_sequence {
  template <size_t T>
  using push_back_t = index_sequence<V..., T>;

  template <size_t H, size_t... T>
  struct print_impl {
    static void print() {
      std::cout << H << std::endl;
      print_impl<T...>::print();
    }
  };

  template <size_t T>
  struct print_impl<T> {
    static void print() { std::cout << T << std::endl; }
  };

  static void print() { print_impl<V...>::print(); }
};

template <size_t N>
struct prime_numbers {
  using search_type = prime_numbers<N * 19 / 20>::type;
  using prime_numbers_prev_t = prime_numbers<N - 1>::type;

  template <class H>
  struct is_prime_number {};

  template <size_t H, size_t... T>
  struct is_prime_number<index_sequence<H, T...>> {
    static constexpr bool value =
        is_prime_number<index_sequence<T...>>::value && !(N % H == 0);
  };

  template <size_t T>
  struct is_prime_number<index_sequence<T>> {
    static constexpr bool value = !(N % T == 0);
  };

  template <bool T>
  struct conditional {};

  template <>
  struct conditional<true> {
    using type = prime_numbers_prev_t::template push_back_t<N>;
  };

  template <>
  struct conditional<false> {
    using type = prime_numbers_prev_t;
  };

  using type = conditional<is_prime_number<prime_numbers_prev_t>::value>::type;
};

template <>
struct prime_numbers<2> {
  using type = index_sequence<2>;
};

int main() {
  static constexpr size_t N = 500;
  prime_numbers<N>::type::print();
  return 0;
}
