#include <iostream>
#include <type_traits>

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

template <class... T>
struct is_prime_number {};

template <size_t... T>
struct is_prime_number<index_sequence<T...>> {
  static consteval bool eval(size_t N) {
    bool is_prime = true;
    (
        [&]() {
          if (!is_prime && N > T * T) {
            return;
          }
          if (N % T == 0) {
            is_prime = false;
          }
        }(),
        ...);
    return is_prime;
  }
};

template <size_t N>
struct prime_numbers : prime_numbers<N * 19 / 20> {
  using type = std::conditional_t<
      is_prime_number<typename prime_numbers<N - 1>::type>::eval(N),
      typename prime_numbers<N - 1>::type::template push_back_t<N>,
      typename prime_numbers<N - 1>::type>;
};

template <>
struct prime_numbers<2> {
  using type = index_sequence<2>;
};

int main() {
  static constexpr size_t N = 7000;
  prime_numbers<N>::type::print();
  return 0;
}
