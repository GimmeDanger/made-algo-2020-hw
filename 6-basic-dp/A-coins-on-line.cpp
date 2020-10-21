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

static constexpr int INVALID_PREV = -1;
static constexpr int MINUS_INF = std::numeric_limits<int>::min();

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  int N, K;
  cin >> N >> K;
  vector<int> coins(N + 1, 0);
  for (int i = 2; i < N; i++) {
    cin >> coins[i];
  }

  // dp[i] = max sum of coins which could be get by reaching position i
  vector<int> dp(N + 1, MINUS_INF);
  // prev[i] = previous position in an optimal solution
  vector<int> prev(N + 1, INVALID_PREV);

  // base
  dp[1] = 0;

  // bottom-up
  for (int i = 2; i <= N; i++) {
    int k_max = i;
    int val_max = dp[k_max];
    // find argmax_j { dp[i - j] }
    for (int j = 1; j <= K && i - j > 0; j++) {
      if (val_max < dp[i - j]) {
        val_max = dp[i - j];
        k_max = i - j;
      }
    }
    dp[i] = val_max + coins[i];
    prev[i] = k_max;
  }

  // compute optimal number of jumps and positions
  size_t jumps = 0;
  stack<int> positions;
  for (int i = N; prev[i] != INVALID_PREV; i = prev[i]) {
    positions.push(i);
    jumps++;
  }
  positions.push(1);

  // output results
  cout << dp[N] << "\n";
  cout << jumps << "\n";
  while (!positions.empty()) {
    cout << positions.top() << " ";
    positions.pop();
  }
  cout << endl;

  return 0;
}
