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

/**
 * Finds root of equation f(x) = 0,
 * where f is a monotonic function
 */
template <class MonotonicFunc>
std::optional<double> find_root(MonotonicFunc f,
                                const int bnd_search_max_iters = 100,
                                const double eps = 1.e-8) {

  // choose bounds [l, r] for a root search
  double l = -1., r = 1.;

  // try to find left bound
  for (int it = 0; it < bnd_search_max_iters && f(l) > 0; it++)
    l *= 2;
  if (f(l) > 0) {
    // left bound is not found
    return {};
  }

  // try to find right bound
  for (int it = 0; it < bnd_search_max_iters && f(r) < 0; it++)
    r *= 2;
  if (f(r) < 0) {
    // right bound is not found
    return {};
  }

  // binary search for a root of f(x) = 0
  for (int it = 0; it < (r - l) / eps && (r - l) > eps; it++) {
    double m = l + (r - l) / 2;
    if (f(m) < 0)
      l = m;
    else
      r = m;
  }
  return r;
}

int main() {
  FAST_IO();
  INPUT_SOURCE();

  double c;
  cin >> c;

  auto f = [c](double x) -> double {
    return x * x + sqrt(x) - c;
  };
  cout << setprecision(6) << fixed << find_root(f).value() << endl;

  return 0;
}
