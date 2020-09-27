#include <bits/stdc++.h>
using namespace std;

#define dbg 0

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

template <class Container>
void print(const Container &c) {
  for (auto it = begin(c); it != end(c); it++)
    cout << *it << ' ';
  if (!c.empty())
    cout << '\n';
}

// Maximal value of array element
constexpr int m_limit = 101;

/**
 * Sorts @arr in ascending order,
 * it's guaranteed that all elements are in range [0, m),
 * Complexity: O(n + m) in time, O(m) in space, where n = len(arr)
 */
template <int m>
void counting_sort(vector<int> &arr) {
  // compute @cnt array such that
  // cnt[i] == number of occurrences of element i in @arr
  array<int, m> cnt{};
  fill(begin(cnt), end(cnt), 0);
  for (int a : arr) {
    assert(a >= 0 && a < m);
    cnt[a]++;
  }
  // refill original array with sorted values
  auto it = begin(arr);
  for (int i = 0; i < m; i++) {
    if (cnt[i]) {
      fill(it, it + cnt[i], i);
      it += cnt[i];
    }
  }
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  // Get input
  vector<int> arr;
  for (int a = 0; cin >> a;)
    arr.push_back(a);

  // Solve
  counting_sort<m_limit>(arr);

  // Output result
  print(arr);

  return 0;
}
