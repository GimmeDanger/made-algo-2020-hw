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

/*
 * Recursive helper, sorts arr[lo, hi) and counts number of inversions in it
 */
template <typename T>
long long get_number_of_inversions_impl(
    int lo, int hi, vector<T> &arr, vector<T> &ws) {

  long long cnt = 0;
  // recursion base: subarray length equals to 1
  if (hi - lo <= 1)
    return cnt;

  int mi = lo + (hi - lo) / 2;
  // recursively sort left half
  cnt += get_number_of_inversions_impl(lo, mi, arr, ws);
  // recursively sort right half
  cnt += get_number_of_inversions_impl(mi, hi, arr, ws);

  // merge two sorted parts
  // arr [lo ... mi-1] [mi ... hi-1]
  //       \/ \/ \/ \/
  // ws  [lo ... hi] - sorted
  // complexity: O(N)
  for (int i = lo, j = mi, k = lo; (i < mi || j < hi) && k < hi; k++) {
    if (i >= mi) {
      ws[k] = arr[j++];
    } else if (j >= hi) {
      ws[k] = arr[i++];
    } else if (arr[i] <= arr[j]) {
      ws[k] = arr[i++];
    } else {
      ws[k] = arr[j++];
      cnt += (mi - i);
    }
  }

  // complexity: O(N)
  copy(begin(ws) + lo, begin(ws) + hi, begin(arr) + lo);
  return cnt;
}

/*
 * Sorts @arr in a non-decreasing order (set by optionally provided comparator),
 * counts the number of inversions: pairs (i, j) such that a_i > a_j and i < j,
 * using merge sort algorithm, complexity: O(NlogN) in time, O(N) in space
 */
template <typename T>
long long get_number_of_inversions(vector<T> &arr) {
  if (arr.empty())
    return 0ll;
  vector<T> workspace(arr.size());
  return get_number_of_inversions_impl(0, arr.size(), arr, workspace);
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

  // Solve & output result
  cout << get_number_of_inversions(arr) << '\n';
  return 0;
}