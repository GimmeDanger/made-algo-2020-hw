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
static constexpr ll A_MULT = 23;
static constexpr ll A_SHIFT = 21563;
static constexpr ll A_MODULUS = 16714589;
static constexpr array<ll, 4> U_COEFFS = {17, 751, 1, 2};
static constexpr array<ll, 4> V_COEFFS = {13, 593, 1, 5};

ll get_mod(ll x, ll modulus) {
  return (modulus + x) % modulus;
}

vector<ll> construct_array(ll n, ll arr0) {
  vector<ll> arr(n, arr0);
  for (size_t i = 1; i < n; i++) {
    arr[i] = get_mod(A_MULT * arr[i - 1] + A_SHIFT, A_MODULUS);
  }
  return arr;
}

void next(pair<ll, ll> &querry, ll r, ll i, ll n) {
  querry.first = U_COEFFS[0] * querry.first + U_COEFFS[1] + U_COEFFS[2] * r + U_COEFFS[3] * i;
  querry.second = V_COEFFS[0] * querry.second + V_COEFFS[1] + V_COEFFS[2] * r + V_COEFFS[3] * i;
  querry.first = get_mod(querry.first, n) + 1;
  querry.second = get_mod(querry.second, n) + 1;
}

class SparseTable {
  const int n;
  const vector<ll> max_pow;
  const vector<vector<ll>> min_val;

  // Complexity: O(N)
  static vector<ll> generate_max_pow(int n) {
    vector<ll> res(n + 1);
    res[0] = -1, res[1] = 0;
    for (size_t i = 2; i <= n; i++) {
      ll k = res[i - 1];
      if ((1ll << (k + 1)) <= i) {
        k++;
      }
      res[i] = k;
    }
    return res;
  }

  // Complexity: O(NlogN)
  static vector<vector<ll>> generate_min_val(const vector<ll> &max_pow,
                                             const vector<ll> &a) {
    const int n = a.size();
    const int m = max_pow[n] + 1;
    vector<vector<ll>> res(n, vector<ll>(m, 0));
    for (size_t k = 0; k < m; k++) {
      for (size_t i = 0; i < n; i++) {
        if (k == 0) {
          res[i][k] = a[i];
        } else {
          res[i][k] = res[i][k - 1];
          if (i + (1ll << (k - 1)) < n)
            res[i][k] = min(res[i][k], res[i + (1ll << (k - 1))][k - 1]);
        }
      }
    }
    return res;
  }

 public:
  // Complexity: O(NlogN)
  explicit SparseTable(const vector<ll> &a)
      : n(a.size())
      , max_pow(generate_max_pow(n))
      , min_val(generate_min_val(max_pow, a))
  {
  }

  // Complexity: O(1)
  ll rmq(ll l, ll r) const {
    assert(l <= r);
    ll res = 0;
    const ll k = max_pow[r - l + 1];
    return min(min_val[l][k], min_val[r - (1ll << k) + 1][k]);
  }
};

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  // Generate array
  ll n, m, a0;
  cin >> n >> m >> a0;
  const auto a = construct_array(n, a0);
  SparseTable st(a);

  // Initial query
  pair<ll, ll> query;
  cin >> query.first >> query.second;

  // Answer queries
  for (ll i = 1; i <= m; i++) {
    ll left = min(query.first, query.second) - 1;
    ll right = max(query.first, query.second) - 1;
    ll r = st.rmq(left, right);
    if (i == m) {
      cout << query.first << " " << query.second << " " << r << "\n";
    }
    next(query, r, i, n);
  }

  return 0;
}
