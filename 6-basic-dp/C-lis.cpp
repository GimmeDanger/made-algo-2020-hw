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

vector<int> computeLIS(const vector<int> &seq) {
  // dp[i] - length of LIS ending at position i
  vector<int> dp(seq.size(), 0);
  vector<int> prev(seq.size(), INVALID_PREV);

  // LIS properties
  int lisLength = 0;
  int lisLastInd = INVALID_PREV;

  for (int i = 0; i < seq.size(); i++) {
    int maxLength = 0, prevInd = INVALID_PREV;
    for (int j = i - 1; j >= 0; j--) {
      if (seq[j] < seq[i] && dp[j] > maxLength) {
        maxLength = dp[j];
        prevInd = j;
      }
    }
    dp[i] = maxLength + 1;
    prev[i] = prevInd;
    // update LIS
    if (dp[i] > lisLength) {
      lisLength = dp[i];
      lisLastInd = i;
    }
  }

  // reconstruct lis
  vector<int> res(lisLength);
  for (int i = lisLength - 1, j = lisLastInd; i >= 0; i--, j = prev[j]) {
    res[i] = seq[j];
  }
  return res;
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  int n;
  cin >> n;
  vector<int> seq(n);
  for (auto &a : seq) {
    cin >> a;
  }

  const auto lis = computeLIS(seq);

  cout << lis.size() << "\n";
  for (auto &a : lis) {
    cout << a << " ";
  }
  cout << endl;

  return 0;
}
