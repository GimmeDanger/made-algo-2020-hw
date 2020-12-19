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

vector<int> compute_zfunction(const string &s) {
    vector<int> z(s.size(), 0);
    if (s.empty()) {
        return z;
    }
    for (int left = 0, right = 0, i = 1; i < s.size(); i++) {
        z[i] = max(0, min(right - i, z[i - left]));
        while (i + z[i] < s.size() and s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
    }
    return z;
}

void solve(istream &is, ostream &os) {
    string s;
    is >> s;
    auto zfunc = compute_zfunction(s);
    for (int i = 1; i < zfunc.size(); i++) {
        os << zfunc[i] << " ";
    }
    os << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
