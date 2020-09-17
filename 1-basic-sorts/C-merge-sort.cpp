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

/*
 * Merge sort recursive helper, sorts arr[lo, hi)
 */
template <typename T, typename Comparator>
void merge_sort_impl(
    int lo, int hi,
    vector<T> &arr, // array to sort
    vector<T> &ws,  // external buffer
    Comparator less_comparator) {

  // recursion base: subarray length equals to 1
  if (hi - lo <= 1)
    return;

  int mi = lo + (hi - lo) / 2;
  // recursively sort left half
  merge_sort_impl(lo, mi, arr, ws, less_comparator);
  // recursively sort right half
  merge_sort_impl(mi, hi, arr, ws, less_comparator);

  // merge two sorted parts
  // arr [lo ... mi-1] [mi ... hi-1]
  //       \/ \/ \/ \/
  // ws  [lo ... hi] - sorted
  // complexity: O(N)
  for (int i = lo, j = mi, k = lo; (i < mi || j < hi) && k < hi; k++) {
    if (j >= hi || (i < mi && j < hi && less_comparator(arr[i], arr[j])))
      ws[k] = arr[i++];
    else
      ws[k] = arr[j++];
  }

  // complexity: O(N)
  copy(begin(ws) + lo, begin(ws) + hi, begin(arr) + lo);
}

/*
 * Sorts @arr in a non-decreasing order (set by optionally provided comparator)
 * using merge sort algorithm, complexity: O(NlogN) in time, O(N) in space
 */
template <typename T, typename Comparator = std::less<T>>
void merge_sort(
    vector<T> &arr, // array to sort
    Comparator less_comparator = Comparator()) {

  if (!arr.empty()) {
    vector<T> workspace(arr.size());
    merge_sort_impl(0, arr.size(), arr, workspace, less_comparator);
  }
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  // Get input
  int n;
  cin >> n;
  vector<int> arr (n);
  for (int &a : arr)
    cin >> a;

  // Solve
  merge_sort(arr);

  // Output result
  print(arr);
  return 0;
}
