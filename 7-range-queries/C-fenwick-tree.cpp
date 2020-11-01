#include <bits/stdc++.h>
using namespace std;

#define dbg 0

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

using ll = long long;

class FenwickTree {
  // Add x to a[i] and update tree
  // Complexity: O(log i)
  void add(int i, ll x) {
    for (; i < a.size(); i = i | (i + 1)) {
      t[i] += x;
    }
  }

  // Compute sum on segment [0, i]
  // Complexity: O(log i)
  ll get(int i) const {
    ll res = 0;
    for (; i >= 0; i = (i & (i + 1)) - 1) {
      res += t[i];
    }
    return res;
  }

 public:
  // Complexity: O(N * log N)
  explicit FenwickTree(vector<ll> a_) : a(std::move(a_)) {
    t.assign(a.size(), 0);
    for (size_t i = 0; i < a.size(); i++) {
      add(i, a[i]);
    }
  }

  // Set x to a[i] and update tree
  // Complexity: O(log i)
  void set(int i, ll x) {
    add(i, x - a[i]);
    a[i] = x;
  }

  // Compute sum on segment [l, r]
  // Complexity: O(log(max (l, r)))
  ll sum(int l, int r) const {
    return l > 0 ? get(r) - get(l - 1) : get(r);
  }

 private:
  vector<ll> a;  //< input array
  vector<ll> t;  //< generalized prefix sums
};

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  int n, i, j;
  cin >> n;
  vector<ll> a(n);
  for (auto &a_i : a) {
    cin >> a_i;
  }

  FenwickTree ft(std::move(a));

  long long x;
  string command;
  while (cin >> command) {
    if (command == "set") {
      cin >> i >> x;
      ft.set(i - 1, x);
    } else if (command == "sum") {
      cin >> i >> j;
      cout << ft.sum(i - 1, j - 1) << '\n';
    }
  }

  return 0;
}
