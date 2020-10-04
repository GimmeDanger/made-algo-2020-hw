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

// Returns the minimum of unimodal  @f
// using ternary search algorithm
template<class UFunc>
double find_minimum(UFunc f, double lo, double hi, const double eps = 1.e-8) {
  long long max_iters = (hi - lo) / eps;
  while (hi - lo > eps && max_iters--) {
    auto m1 = lo + (hi - lo) / 3;
    auto m2 = hi - (hi - lo) / 3;
    if (f(m1) < f(m2))
      hi = m2;
    else
      lo = m1;
  }
  return hi;
}

/** Scheme of our problem
         P (start)
         |
         |_ _ _ _*_ _ _ _ _
             x             |
          <----->          |  a
                           |
                           F (finish)

      T = sqrt((1-a)^2 + x^2) / p + sqrt(a^2 + (1-x)^2) / f -> min
*/
int main() {
  FAST_IO();
  INPUT_SOURCE();
  double p, f, a;
  cin >> p >> f >> a;
  auto required_time = [p, f, a](double x) {
    auto len_p = sqrt((1 - a) * (1 - a) + x * x);
    auto len_f = sqrt(a * a + (1 - x) * (1 - x));
    return len_p / p + len_f / f;
  };
  cout << find_minimum(required_time, 0., 1.) << endl;
  return 0;
}
