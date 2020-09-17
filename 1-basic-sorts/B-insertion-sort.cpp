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
 * Sorts @arr in a non-decreasing order (set by optionally provided comparator)
 * using insertion sort algorithm, complexity: O(N^2) in the worst case
 */
template <typename T, typename Comparator = std::less<T>>
void insertion_sort(vector<T> &arr, Comparator less_comparator = Comparator()) {
  if (arr.empty())
    return;
  // Invariant:
  // [ * * * * | x | ? ? ? ? ? ]
  //    sorted        unknown
  for (int i = 1; i < arr.size(); i++)
    for (int j = i - 1; j >= 0 && less_comparator(arr[j + 1], arr[j]); j--)
      swap(arr[j], arr[j + 1]);
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
  insertion_sort(arr);

  // Output result
  print(arr);
  return 0;
}
