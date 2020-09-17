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

// if subarray size on recursion call is less than cutoff value,
// switch to insertion sort algorithm for optimization
constexpr int cutoff = 15;

/*
 * Sorts @arr in a non-decreasing order (set by optionally provided comparator)
 * using insertion sort algorithm, complexity: O(N^2) in the worst case
 */
template <typename T, typename Comparator>
void insertion_sort(vector<T> &arr, int lo, int hi, Comparator comp) {
  if (hi - lo <= 1)
    return;
  // Invariant:
  // [ * * * * | x | ? ? ? ? ? ]
  //    sorted        unknown
  for (int i = lo + 1; i < hi; i++)
    for (int j = i - 1; j >= lo && comp(arr[j + 1], arr[j]); j--)
      swap(arr[j], arr[j + 1]);
}

/*
 * Partitions arr[lo, hi) into tree parts:
 * [ < arr[lo]  ] [ = arr[lo] ] [ > arr[lo] ]
 *   /\             /\           /\           /\
 *   lo             lt           gt           hi
 */
template <typename T, typename Comparator>
void partition_3_way(
    vector<T> &arr,
    int lo, int hi,
    int &lt, int &gt,
    Comparator comp) {

  lt = lo, gt = hi;
  // Invariant during the loop iterations
  // [ (<) (<) (<) ]  [ (=) (=) (=) ] [ (?) (?) (?) ]  [ (>) (>) (>) ]
  //                    lt               i               gt
  for (int i = lo, x = arr[lo]; i < gt;) {
    if (comp(arr[i], x))
      swap(arr[lt++], arr[i++]);
    else if (comp(x, arr[i]))
      swap(arr[i], arr[--gt]);
    else
      i++;
  }
}

/*
 * Merge sort recursive helper, sorts arr[lo, hi)
 */
template <typename T, typename Comparator>
void quick_sort_impl(vector<T> &arr, int lo, int hi, Comparator comp) {
  // recursion base: cutoff to insertion sort
  if (hi - lo <= cutoff) {
    insertion_sort(arr, lo, hi, comp);
    return;
  }

  int lt, gt;
  // partition into 3 parts: [ < ] [ = ] [ > ]
  partition_3_way(arr, lo, hi, lt, gt, comp);

  // recursively sort left & right halves
  quick_sort_impl(arr, lo, lt, comp);
  quick_sort_impl(arr, gt, hi, comp);
}

/*
 * Sorts @arr in a non-decreasing order (set by optionally provided comparator)
 * using quick sort algorithm, complexity: O(NlogN) in time
 * Used optimizations:
 * 1) cutoff to insertion sort,
 * 2) random shuffle,
 * 3) 3-way partition
 */
template <typename T, typename Comparator = std::less<T>>
void quick_sort(vector<T> &arr, Comparator comp = Comparator()) {
  if (!arr.empty()) {
    // shuffle input array for a probabilistic
    // guarantee of sublinear time complexity
    random_device rd;
    mt19937 gen(rd());
    shuffle(begin(arr), end(arr), gen);
    // sort arr[0, len-1]
    quick_sort_impl(arr, 0, arr.size(), comp);
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
  quick_sort(arr);

  // Output result
  print(arr);
  return 0;
}
