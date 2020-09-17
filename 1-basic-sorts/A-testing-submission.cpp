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

int main() {
  FAST_IO();
  INPUT_SOURCE();

  int t;
  cin >> t;
  while (t--) {
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
  }
  return 0;
}