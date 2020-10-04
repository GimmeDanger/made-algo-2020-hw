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

// Returns the lowest value such that f(val) = true
// f is considered to be monotonically non-decreasing
//                      f(x) = true
//                   ----------------
// ------------------
//     f(x) = false
template<class Func>
long long bin_search(Func f, long long lo, long long hi) {
  while (lo <= hi) {
    auto mi = lo + (hi - lo) / 2;
    if (f(mi) == false)
      lo = mi + 1;
    else
      hi = mi - 1;
  }
  return lo;
}

// Check if we could construct @n pages in time @t
bool could_construct(long long n, long long x, long long y, long long t) {
  // we need to find minimum of function
  // f = max(x * m, y * (n - 1 - m)), where m in [0, n-1]
  // and check if its less than t
  // this function has a graphic of form:
  // \       /
  //  \    /
  //   \ /
  //   m such that x * m = y * (n - 1 - m) <=>
  //   <=> (x + y) * m = y * (n - 1)
  //   <=> m = y * (n + 1) / (x + y)
  auto check_m = [n, x, y, t](long long m) {
    auto rec = max<long long>(x * m, y * (n - 1 - m));
    if (rec > 0 && rec <= t) {
      return true;
    }
    return false;
  };
  auto m1 = y * (n - 1) / (x + y);
  auto m2 = m1 + 1; //< upper bound
  return check_m(m1) || check_m(m2);
}

int solve(long long n, long long x, long long y) {
  // returns true, if at least n copies
  // could be printed in time t
  auto f = [n, x, y](long long t) {
    // first, try the fastest copier
    // to make the second page
    if (n <= 1)
      return t >= min(x, y);
    t -= min(x, y);
    if (t <= 0)
      return false;
    return could_construct(n, x, y, t);
  };
  auto res = bin_search(f, 0, n * min(x, y));
  return res;
}

int main() {
  FAST_IO();
  INPUT_SOURCE();
  long long n, x, y;
  cin >> n >> x >> y;
  cout << solve(n, x, y) << endl;
  return 0;
}
