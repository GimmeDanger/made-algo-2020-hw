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

template<typename T>
int upperBound(const vector<T> &arr, T x) {
  return lowerBound(arr, x + 1);
}

template<typename T>
int rangeBetween(const vector<T> &arr, T l, T r) {
  return upperBound(arr, r) - lowerBound(arr, l);
}

int main() {
  FAST_IO();
  INPUT_SOURCE();

  // get input
  int n, k, l, r;
  cin >> n;
  vector<int> arr(n);
  for (auto &a : arr) {
    cin >> a;
  }
  sort(begin(arr), end(arr));

  // process query
  cin >> k;
  while (k--) {
    cin >> l >> r;
    cout << rangeBetween(arr, l, r) << " ";
  }
  return 0;
}
