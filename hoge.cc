#include <iostream>
#include <type_traits>
#include <utility>

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

template <size_t N, size_t... T>
constexpr auto operator+(index_sequence<T...>,
                         std::integral_constant<size_t, N>)
    -> std::conditional_t<N == 0, index_sequence<T...>,
                          index_sequence<T..., N>>;

template <int... T>
void print(std::index_sequence<T...> /*unused*/) {
  for (auto&& i : {T...}) {
    std::cout << i << std::endl;
  }
}

template <size_t... V>
struct remove_zero {
  using type = decltype((std::declval<index_sequence<>>() + ... +
                         std::declval<std::integral_constant<size_t, V>>()));
};

int main() {
  remove_zero<1, 3, 0, 2, 0, 1>::type::print();
  return 0;
}
