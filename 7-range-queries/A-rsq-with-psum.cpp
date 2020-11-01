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
static constexpr ll A_MODULUS = (1ll << 16);
static constexpr ll B_MODULUS = (1ll << 30);

vector<ll> construct_array(ll n, ll x, ll y, ll arr0, ll mod) {
  vector<ll> arr(n, arr0);
  for (size_t i = 1; i < n; i++) {
    arr[i] = (mod + x * arr[i - 1] + y) % mod;
  }
  return arr;
}

vector<ll> compute_prefixes(const vector<ll> &arr) {
  vector<ll> psum(arr.size(), arr[0]);
  for (size_t i = 1; i < psum.size(); i++) {
    psum[i] = arr[i] + psum[i - 1];
  }
  return psum;
}

pair<ll, ll> get_query(const vector<ll> &c, ll i) {
  return {min(c[2 * i], c[2 * i + 1]), max(c[2 * i], c[2 * i + 1])};
}

ll rsq(const vector<ll> &psum, ll l, ll r) {
  return l ? psum[r] - psum[l - 1] : psum[r];
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  // Generate array
  // and compute prefix sums
  ll n, x, y, a0, a_len;
  cin >> n >> x >> y >> a0;
  a_len = n;
  const auto a = construct_array(a_len, x, y, a0, A_MODULUS);
  const auto psum = compute_prefixes(a);

  // Generate queries data
  ll m, z, t, b0, b_len;
  cin >> m >> z >> t >> b0;
  b_len = 2 * m + 1;
  auto c = construct_array(b_len, z, t, b0, B_MODULUS);
  for (auto &c_i : c) {
    c_i = (n + c_i) % n;
  }

  // Answer queries
  long long res = 0;
  for (size_t i = 0; i < m; i++) {
    auto q = get_query(c, i);
    res += rsq(psum, q.first, q.second);
  }
  cout << res << endl;

  return 0;
}
