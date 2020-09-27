#include <bits/stdc++.h>
using namespace std;

int partition(vector<int> &arr, int lo, int hi) {
  // [  A[k] <= x   |   A[k] > x  | unrestricted | x      ]
  //   lo <= k <= i |  i < k < j  |              | k == r
  int x = arr[hi];
  int i = lo - 1;
  for (int j = lo; j <= hi - 1; j++) {
    if (arr[j] <= x)
      swap(arr[++i], arr[j]);
  }
  swap(arr[++i], arr[hi]);
  return i;
}

int randomize_partition(vector<int> &arr, int lo, int hi) {
  int pivot = lo + rand() % (hi + 1 - lo);
  swap(arr[pivot], arr[hi]);
  return partition(arr, lo, hi);
}

/**
 * Finds k-th smallest element in @arr
 * Iterative implementation of quick select algorithm,
 * Complexity: O(N) on avg, O(N^2) in the worst case, where N = len(@arr)
 */
int quick_select(vector<int> arr, int k) {
  int lo = 0, hi = arr.size() - 1;
  while (lo <= hi) {
    int pivot = randomize_partition(arr, lo, hi);
    if (pivot == k)
      return arr[k];
    else if (pivot < k)
      lo = pivot + 1;
    else if (pivot > k)
      hi = pivot - 1;
  }
  // unreachable
  assert(0);
  return -1;
}

int main() {

  int n;
  cin >> n;
  vector<int> arr(n);
  vector<int> ws(n);
  for (int i = 0; i < n; i++)
    cin >> arr[i];

  int m;
  cin >> m;
  while (m--) {
    ws.clear();
    int i, j, k;
    cin >> i >> j >> k;
    i--; j--; k--;
    copy(begin(arr) + i, begin(arr) + j + 1, back_inserter(ws));
    cout << quick_select(ws, k) << '\n';
  }

  return 0;
}
