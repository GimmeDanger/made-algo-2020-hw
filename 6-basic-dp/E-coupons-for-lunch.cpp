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

static constexpr int MIN_CHECK_FOR_COUPON = 101;

enum class move_t {
  USE_COUPON,
  GET_COUPON,
  DO_NOTHING,
  INVALID
};

// Dynamic programming state
struct State {
  int spent = numeric_limits<int>::max();
  move_t move_type = move_t::INVALID;
  bool valid = false;
};

int main() {
  FAST_IO();
  INPUT_SOURCE();

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &p_i : p) {
    cin >> p_i;
  }

  // dp[i][j] == optimal state on day i
  // if you have j coupons available
  vector<vector<State>> dp(n + 1, vector<State>(n + 1));

  // base
  dp[0][0].valid = true;
  dp[0][0].spent = 0;

  // bottom-up dynamic programming
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= n; j++) {
      if (!dp[i][j].valid) {
        continue;
      }
      // try all possible moves
      if (j > 0 && dp[i + 1][j - 1].spent >= dp[i][j].spent) {
        // spend coupon if you can
        dp[i + 1][j - 1].spent = dp[i][j].spent;
        dp[i + 1][j - 1].move_type = move_t::USE_COUPON;
        dp[i + 1][j - 1].valid = true;
      }
      if (p[i] < MIN_CHECK_FOR_COUPON) {
        if (dp[i + 1][j].spent >= (dp[i][j].spent + p[i])) {
          // do nothing if you can
          dp[i + 1][j].spent = dp[i][j].spent + p[i];
          dp[i + 1][j].move_type = move_t::DO_NOTHING;
          dp[i + 1][j].valid = true;
        }
      } else {
        if (j < n && dp[i + 1][j + 1].spent >= (dp[i][j].spent + p[i])) {
          // get coupon if you can
          dp[i + 1][j + 1].spent = dp[i][j].spent + p[i];
          dp[i + 1][j + 1].move_type = move_t::GET_COUPON;
          dp[i + 1][j + 1].valid = true;
        }
      }
    }
  }

  // reconstruct answer
  int opt_state_ind = 0;
  int min_spent = numeric_limits<int>::max();
  for (int j = 0; j <= n; j++) {
    auto &s = dp[n][j];
    if (s.valid && s.spent <= min_spent) {
      min_spent = s.spent;
      opt_state_ind = j;
    }
  }
  cout << min_spent << "\n";

  stack<int> st_coupon_usage_days;
  for (int i = n, j = opt_state_ind; i >= 0; i--) {
    switch (dp[i][j].move_type) {
      case move_t::USE_COUPON:
        st_coupon_usage_days.push(i);
        j++;
        break;
      case move_t::GET_COUPON:
        j--;
        break;
      default:
        break;
    }
  }
  cout << opt_state_ind << " " << st_coupon_usage_days.size() << "\n";
  while (!st_coupon_usage_days.empty()) {
    cout << st_coupon_usage_days.top() << "\n";
    st_coupon_usage_days.pop();
  }
  return 0;
}
