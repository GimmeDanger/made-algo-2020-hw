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

static inline int pickMinimum(int a, int b, int c) { return min(min(a, b), c); }

// O(N * M) in time and O(min(N, M)) in space
size_t computeEditDistance(const string &s1, const string &s2) {
  const auto &x = s1.size() > s2.size() ? s1 : s2;
  const auto &y = s1.size() > s2.size() ? s2 : s1;
  const int rows = x.size(), cols = y.size();
  vector<int> dp(cols + 1, 0);
  for (int r = 0; r <= rows; r++) {
    int prev_diag = -1;
    for (int c = 0; c <= cols; c++) {
      int opt = 0;
      if (r == 0 || c == 0) {
        opt = max(r, c);
      } else if (x[r - 1] == y[c - 1]) {
        opt = prev_diag;
      } else {
        opt = 1 + pickMinimum(prev_diag /*replace*/,
                              dp[c - 1] /*insert*/,
                              dp[c] /*delete*/);
      }
      prev_diag = dp[c];
      dp[c] = opt;
    }
  }
  return dp[cols];
}

// O(N * M) in time and O(N * M) in space
size_t computeEditDistanceNaive(const string &x, const string &y) {
  const int rows = x.size(), cols = y.size();
  vector<vector<int>> dp(rows + 1, vector<int>(cols + 1, 0));
  for (int r = 0; r <= rows; r++) {
    for (int c = 0; c <= cols; c++) {
      if (r == 0 || c == 0) {
        dp[r][c] = max(r, c);
      } else if (x[r - 1] == y[c - 1]) {
        dp[r][c] = dp[r - 1][c - 1];
      } else {
        dp[r][c] =
            1 + pickMinimum(dp[r - 1][c - 1] /*replace*/,
                            dp[r - 1][c] /*delete*/,
                            dp[r][c - 1] /*insert*/);
      }
    }
  }
  return dp[rows][cols];
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  string s1, s2;
  cin >> s1 >> s2;
  cout << computeEditDistance(s1, s2) << endl;

  return 0;
}
