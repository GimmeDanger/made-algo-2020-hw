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

/**
 * Returns position of the first element that is
 * not less than value, or last if no such element is found.
 */
template<typename T>
int lowerBound(const vector<T> &arr, T x) {
  // assert(is_sorted(begin(arr), end(arr)));
  int lo = -1, hi = arr.size();
  while (lo < hi - 1) {
    int mi = lo + (hi - lo) / 2;
    if (arr[mi] < x)
      lo = mi;
    else
      hi = mi;
  }
  return hi;
}

/**
 * Finds element in @arr, which is closest to x,
 * returns nullopt for an empty array
 */
template <typename T>
std::optional<T> approximateBinarySearch(const vector<T> &arr, T x) {
  if (arr.empty())
    return {};
  int lb = lowerBound(arr, x);
  if (lb > 0 && lb < arr.size()) {
    // if lb position is in the middle, we need to check previous
    // and choose the closest element to x
    if (abs(arr[lb] - x) < abs(arr[lb - 1] - x))
      return optional<T>{arr[lb]};
    else
      return optional<T>{arr[lb - 1]};
  } else if (lb == 0) {
    // lb -- is the closest because the use lower bound
    return optional<T>{arr[lb]};
  } else if (lb == arr.size()) {
    // lb-1 -- is the closest because the use lower bound
    return optional<T>{arr[lb - 1]};
  }
}

int main() {
  FAST_IO();
  INPUT_SOURCE();

  // get input
  int n, k, q;
  cin >> n >> k;
  vector<int> arr(n);
  for (auto &a : arr) {
    cin >> a;
  }

  // process query
  while (k--) {
    cin >> q;
    cout << approximateBinarySearch(arr, q).value() << '\n';
  }
  return 0;
}
