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

vector<int> compute_pfunction(const string& s) {
    vector<int> p(s.size());
    if (s.empty()) {
        return p;
    }
    p[0] = 0;
    for (int i = 1; i < s.size(); i++) {
        int k = p[i - 1];
        while (k > 0 && s[i] != s[k]) {
            k = p[k - 1];
        }
        if (s[i] == s[k]) {
            k++;
        }
        p[i] = k;
    }
    return p;
}

vector<int> kmp_substring(const string &p, const string &t) {
    const string s = p + '#' + t;
    auto pfunc = compute_pfunction(s);
    vector<int> start_pos;
    for (int i = p.size() + 1; i < s.size(); i++) {
        if (pfunc[i] == p.size()) {
            start_pos.push_back(i - 2 * p.size());
        }
    }
    return start_pos;
}

void solve(istream &is, ostream &os) {
    string p, t;
    is >> p >> t;
    auto start_pos = kmp_substring(p, t);
    os << start_pos.size() << endl;
    for (int pos : start_pos) {
        os << pos + 1 << " ";
    }
    os << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
