//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03
// REQUIRES: diagnose-if-support

// Test that libc++ generates a warning diagnostic when the container is
// provided a non-const callable comparator or a non-const hasher.

#include <__type_traits/invoke.h>
#include <unordered_set>
#include <unordered_map>

struct BadHash {
  template <class T>
  std::size_t operator()(T const& t) {
    return std::hash<T>{}(t);
  }
};

struct BadEqual {
  template <class T, class U>
  bool operator()(T const& t, U const& u) {
    return t == u;
  }
};

void f() {
  static_assert(!std::__is_invocable_v<BadEqual const&, int const&, int const&>, "");
  static_assert(std::__is_invocable_v<BadEqual&, int const&, int const&>, "");

  // expected-warning@unordered_set:* 2 {{the specified comparator type does not provide a viable const call operator}}
  // expected-warning@unordered_map:* 2 {{the specified comparator type does not provide a viable const call operator}}
  // expected-warning@unordered_set:* 2 {{the specified hash functor does not provide a viable const call operator}}
  // expected-warning@unordered_map:* 2 {{the specified hash functor does not provide a viable const call operator}}

  {
    using C = std::unordered_set<int, BadHash, BadEqual>;
    C s;
  }
  {
    using C = std::unordered_multiset<long, BadHash, BadEqual>;
    C s;
  }
  {
    using C = std::unordered_map<int, int, BadHash, BadEqual>;
    C s;
  }
  {
    using C = std::unordered_multimap<long, int, BadHash, BadEqual>;
    C s;
  }
}
