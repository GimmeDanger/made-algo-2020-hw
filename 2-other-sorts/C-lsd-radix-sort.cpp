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
void print(const Container &c, const char delim = ' ') {
  for (auto it = begin(c); it != end(c); it++)
    cout << *it << delim;
  if (!c.empty())
    cout << '\n';
}

constexpr int alphabet_begin = 'a';
constexpr int alphabet_end = 'z' + 1;

/**
 *  Runs k tours of lsd (least significant digit) radix sort,
 *  if k >= 0 || k > m entire sorting procedure will be performed,
 *  complexity: O(k x (n + radix)) in time, O(n + radix) in space,
 *  where radix -- alphabet_size, n -- array size, m -- string size
 */
template <int alphabet_begin, int alphabet_end>
void lsd_radix_sort(
    vector<string> &arr, //< array to sort
    int m,               //< fixed length of strings
    int k = -1           //< required number of digits to sort (all if default)
) {
  const int radix = alphabet_end - alphabet_begin;
  if (arr.empty())
    return;

  // determine required number of tours
  k = (k >= 0 && k <= m) ? k : m;

  // allocate auxiliary array for counting sort subroutine
  const int n = arr.size();
  vector<string> aux(n);

  // index getter helper
  auto get_pos = [](char ch) {
    return ch - alphabet_begin;
  };

  // run k tours of counting sort starting from least significant digit
  array<int, radix + 1> cnt{};
  for (int d = m - 1; d >= (m - k); d--) {
    // stage 1: compute digit occurrences, O(n)
    fill(begin(cnt), end(cnt), 0);
    for (auto &s : arr)
      cnt[get_pos(s[d]) + 1]++;
    // stage 2: compute digit offsets, O(radix)
    for (int i = 0; i < radix; i++)
      cnt[i + 1] += cnt[i];
    // stage 3: get sorted order, O(n)
    for (auto &s : arr)
      aux[cnt[get_pos(s[d])]++] = move(s);
    // stage 4: move back result, O(1)
    swap(arr, aux);
  }
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  // Get input
  int n, m, k;
  cin >> n >> m >> k;
  vector<string> arr (n);
  for (auto &s : arr)
    cin >> s;

  // Solve
  lsd_radix_sort<alphabet_begin, alphabet_end>(arr, m, k);

  // Output result
  print(arr, '\n');
  return 0;
}
