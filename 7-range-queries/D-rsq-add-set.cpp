#include <bits/stdc++.h>
using namespace std;

#define dbg 1

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

class SegmentTree {

  static size_t compute_x(size_t n) {
    size_t x = 1;
    while (x < n) {
      x <<= 1u;
    }
    return x;
  }

  static size_t parrent(size_t v) {
    return (v - 1) / 2;
  }

  static size_t left(size_t v) {
    return 2 * v + 1;
  }

  static size_t right(size_t v) {
    return 2 * v + 2;
  }

  size_t tree_ind(size_t arr_ind) const {
    return (arr_ind + x - 1);
  }

  // TODO: for add, set and rsq
  ll get(size_t v, size_t l, size_t r) const {
    if (upd_set[v].has_value()) {
      return (r - l + 1) * upd_set[v].value();
    }
    else {
      return t[v] + (r - l + 1) * upd_add[v];
    }
  }

  void update_by_add_mark(size_t ch, size_t p) {
    if (upd_set[ch].has_value()) {
      upd_set[ch].value() += upd_add[p];
    } else {
      upd_add[ch] += upd_add[p];
    }
  }

  void update_by_set_mark(size_t ch, size_t p) {
    upd_set[ch] = upd_set[p].value();
    upd_add[ch] = 0;
  }

  // TODO: for add, set and rsq
  void push(size_t v, size_t l, size_t r) {
    if (l == r /*v -- leaf*/) {
      if (upd_set[v].has_value()) {
        t[v] = upd_set[v].value();
      } else {
        t[v] += upd_add[v];
      }
    }
    else {
      if (upd_set[v].has_value()) {
        update_by_set_mark(left(v), v);
        update_by_set_mark(right(v), v);
      }
      else {
        update_by_add_mark(left(v), v);
        update_by_add_mark(right(v), v);
      }
      size_t m = l + (r - l) / 2;
      t[v] = get(left(v), l, m) + get(right(v), m + 1, r);
    }
    upd_add[v] = 0;
    upd_set[v] = {};
  }

  void set_impl(size_t v, size_t l, size_t r, size_t a, size_t b, ll val) {
    push(v, l, r);
    if (l > b || r < a) {
      return;
    }
    if (l >= a && r <= b) {
      upd_set[v] = {val};
      // push reset upd's of v
      // so upd_add[v] = 0;
      return;
    }
    size_t m = l + (r - l) / 2;
    set_impl(left(v), l, m, a, b, val);
    set_impl(right(v), m + 1, r, a, b, val);
    push(v, l, r);
  }

  void add_impl(size_t v, size_t l, size_t r, size_t a, size_t b, ll val) {
    push(v, l, r);
    if (l > b || r < a) {
      return;
    }
    if (l >= a && r <= b) {
      upd_add[v] = val;
      // push reset upd's of v
      // so upd_set[v] = {};
      return;
    }
    size_t m = l + (r - l) / 2;
    add_impl(left(v), l, m, a, b, val);
    add_impl(right(v), m + 1, r, a, b, val);
    push(v, l, r);
  }

  ll rsq_impl(size_t v, size_t l, size_t r, size_t a, size_t b) {
    push(v, l, r);
    if (l > b || r < a) {
      return 0;
    }
    if (l >= a && r <= b)
      return get(v, l, r);
    size_t m = l + (r - l) / 2;
    return (rsq_impl(left(v), l, m, a, b) + rsq_impl(right(v), m + 1, r, a, b));
  }

 public:
  // Complexity: O(n)
  explicit SegmentTree(const vector<ll> &arr)
    : n(arr.size())
    , x(compute_x(n))
  {
    t.resize(2 * x - 1, 0);
    upd_add.resize(2 * x - 1, 0);
    upd_set.resize(2 * x - 1, {});
    for (size_t i = 0; i < n; i++) {
      t[i + (x - 1)] = arr[i];
    }
    for (ll v = x - 2; v >= 0; v--) {
      t[v] = t[left(v)] + t[right(v)];
    }
  }

  // Set val to all elements on segment [l, r]
  // Complexity: O(log n)
  void set(size_t l, size_t r, ll val) {
    set_impl(0, 0, x - 1, l, r, val);
  }

  // Add val to all elements on segment [l, r]
  // Complexity: O(log n)
  void add(size_t l, size_t r, ll val) {
    add_impl(0, 0, x - 1, l, r, val);
  }

  // Compute sum on segment [l, r]
  // Complexity: O(log n)
  ll rsq(size_t l, size_t r) {
    return rsq_impl(0, 0, x - 1, l, r);
  }

  ll get_n() const {
    return n;
  }

 private:
  const ll n; //< input array size
  const ll x; //< min power of 2 >= n
  vector<ll> t; //< tree flat representation
  vector<ll> upd_add; //< tree update marks for add operation
  vector<optional<ll>> upd_set; //< tree update marks for set operation
};

// All operation in O(N),
// used for testing
class SegmentTreeNaive {
  vector<ll> arr;

 public:
  explicit SegmentTreeNaive(const vector<ll> arr_)
    : arr(arr_)
  {
  }

  ll rsq(size_t l, size_t r) const {
    ll res = 0;
    for (size_t i = l; i <= r; i++) {
      res += arr[i];
    }
    return res;
  }

  ll rmq(size_t l, size_t r) const {
    ll res = numeric_limits<ll>::max();
    for (size_t i = l; i <= r; i++) {
      res = min(res, arr[i]);
    }
    return res;
  }

  void add(size_t l, size_t r, ll val) {
    for (size_t i = l; i <= r; i++) {
      arr[i] += val;
    }
  }

  void set(size_t l, size_t r, ll val) {
    for (size_t i = l; i <= r; i++) {
      arr[i] = val;
    }
  }
};

void validate_state(const SegmentTree &st, const SegmentTreeNaive &stn) {
  SegmentTree st_ = st;
  SegmentTreeNaive stn_ = stn;
  for (size_t i = 0; i < st.get_n(); i++) {
    auto a_i = st_.rsq(i, i);
    auto a_i_naive = stn_.rsq(i, i);
    assert(a_i == a_i_naive);
  }
}

void process_queries(istream &is, ostream &os, SegmentTree &st, SegmentTreeNaive &stn) {
  ll i, j, x;
  string command;
  while (is >> command) {
    if (command == "add") {
      is >> i >> j >> x;
      st.add(i - 1, j - 1, x);
      if (dbg) {
        stn.add(i - 1, j - 1, x);
        validate_state(st, stn);
      }
    }
    if (command == "set") {
      is >> i >> j >> x;
      st.set(i - 1, j - 1, x);
      if (dbg) {
        stn.set(i - 1, j - 1, x);
        validate_state(st, stn);
      }
    } else if (command == "sum") {
      is >> i >> j;
      auto res = st.rsq(i - 1, j - 1);
      if (dbg) {
        auto res_naive = stn.rsq(i - 1, j - 1);
        assert(res == res_naive);
        validate_state(st, stn);
      }
      os << res << endl;
    }
  }
}

void fuzzy_testing() {
  static constexpr int TESTS = 1000;
  static constexpr int QUERIES = 50;
  static constexpr int MIN_N = 1;
  static constexpr int MAX_N = 50;
  static constexpr int MIN_VAL = -10;
  static constexpr int MAX_VAL = +10;
  static constexpr int OPS_N = 3;
  std::random_device rand_dev;
  std::mt19937 gen (rand_dev ());
  std::uniform_int_distribution<int> n_distr (MIN_N, MAX_N);
  std::uniform_int_distribution<int> val_distr (MIN_VAL, MAX_VAL);
  for (int t = 0; t < TESTS; t++) {
    vector<ll> a(n_distr(gen));
    for (auto &a_i : a) {
      a_i = val_distr(gen);
    }
    SegmentTree st(a);
    SegmentTreeNaive stn(a);
    vector<string> queries;
    for (int q = 0; q < QUERIES; q++) {
      stringstream is, os;
      int q_type = rand() % OPS_N; // add, set or rsq
      auto l = rand() % a.size();
      auto r = l + rand() % (a.size() - l);
      l++; r++;
      if (q_type == 0) {
        is << "sum " << l << " " << r  << "\n";
      }
      else if (q_type == 1) {
        is << "add " << l << " " << r << " " << val_distr(gen) << "\n";
      }
      else if (q_type == 2){
        is << "set " << l << " " << r << " " << val_distr(gen) << "\n";
      }
      queries.push_back(is.str());
      process_queries(is, os, st, stn);
    }
  }
  cout << "All tests passed!" << endl;
}

void solve(istream &is, ostream &os) {
  assert(!dbg);
  int n;
  cin >> n;
  vector<ll> a(n);
  for (auto &a_i : a) {
    cin >> a_i;
  }
  SegmentTree st(a);
  SegmentTreeNaive stn({});
  process_queries(is, os, st, stn);
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  if (dbg) {
    fuzzy_testing();
  }
  else {
    solve(cin, cout);
  }

  return 0;
}
