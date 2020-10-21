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

static constexpr char INVALID_MOVE = 0;
static constexpr char TURN_RIGHT = 'R';
static constexpr char TURN_DOWN = 'D';
static constexpr int MINUS_INF = std::numeric_limits<int>::min();

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  int rows, cols, coins;
  cin >> rows >> cols;

  vector<vector<char>> prev_move(rows + 1,
                                 vector<char>(cols + 1, INVALID_MOVE));
  vector<vector<int>> dp(rows + 1, vector<int>(cols + 1, MINUS_INF));

  // buttom-up approach
  for (int r = 1; r <= rows; r++) {
    for (int c = 1; c <= cols; c++) {
      cin >> coins;
      if (r == 1 && c == 1) {
        dp[r][c] = coins;
      } else if (dp[r][c - 1] > dp[r - 1][c]) {
        dp[r][c] = dp[r][c - 1] + coins;
        prev_move[r][c] = TURN_RIGHT;
      } else {
        dp[r][c] = dp[r - 1][c] + coins;
        prev_move[r][c] = TURN_DOWN;
      }
    }
  }

  // reconstruct an optimal path
  stack<char> moves;
  for (int r = rows, c = cols; prev_move[r][c] != INVALID_MOVE;) {
    moves.push(prev_move[r][c]);
    if (moves.top() == TURN_RIGHT) {
      c--;
    } else {
      r--;
    }
  }

  // output results
  cout << dp[rows][cols] << "\n";
  while (!moves.empty()) {
    cout << moves.top();
    moves.pop();
  }
  cout << endl;

  return 0;
}
