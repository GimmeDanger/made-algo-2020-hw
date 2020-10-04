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

// Returns the lowest value such that f(val) = false
// f is considered to be monotonically decreasing
//    f(x) = true
// ----------------
//                 ------------------
//                     f(x) = false
template<class Func>
long long solve(Func f, long long lo, long long hi) {
  while (lo <= hi) {
    auto mi = lo + (hi - lo) / 2;
    if (f(mi) == true)
      lo = mi + 1;
    else
      hi = mi - 1;
  }
  return lo;
}

constexpr int d_rope_limit_min = 1;
constexpr int d_rope_limit_max = 10'000'000;

int main() {
  FAST_IO();
  INPUT_SOURCE();

  int n, k;
  cin >> n >> k;
  vector<int> ropes(n);
  for (auto &r : ropes) {
    cin >> r;
  }

  // returns true, if at least k ropes of length d
  // could be cut from the vector of ropes
  auto f = [&ropes, k](int d) {
    int ropes_of_required_len = 0;
    for (auto &r : ropes) {
      ropes_of_required_len += r / d;
      if (ropes_of_required_len >= k)
        return true;
    }
    return (ropes_of_required_len >= k);
  };

  cout << solve(f, d_rope_limit_min, d_rope_limit_max) - 1 << endl;

  return 0;
}
