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

using ll = long long;
static constexpr ll p = 101;
static constexpr ll m = 100030001;
static_assert(m * m < numeric_limits<ll>::max(), "sanity_check");

void init(const string &s, vector<ll>& p_hash, vector<ll>& p_pows) {
    p_hash.resize(s.size());
    p_pows.resize(s.size());
    if (s.empty()) {
        return;
    }
    p_hash[0] = s[0];
    p_pows[0] = 1;
    for (int i = 1; i < s.size(); i++) {
        p_hash[i] = (p_hash[i - 1] * p + s[i]) % m;
        p_pows[i] = (p_pows[i - 1] * p) % m;
    }
}

ll get_substr_hash(const vector<ll>& p_hash, const vector<ll>& p_pows, int lo, int hi) {
    if (lo == 0) {
        return p_hash[hi];
    }
    return (p_hash[hi] + (m - (p_hash[lo - 1] * p_pows[hi - lo + 1]) % m) % m) % m;
}

bool is_equal(const string view, const vector<ll>& p_hash, const vector<ll>& p_pows, int lo_1, int hi_1, int lo_2, int hi_2) {
    if (hi_1 - lo_1 != hi_2 - lo_2 ||
        get_substr_hash(p_hash, p_pows, lo_1, hi_1) != get_substr_hash(p_hash, p_pows, lo_2, hi_2)) {
        return false;
    }
    return view.substr(lo_1, hi_1 - lo_1 + 1) == view.substr(lo_2, hi_2 - lo_2 + 1);
}

void solve(istream &is, ostream &os) {
    int n;
    string s;
    is >> s >> n;

    vector<ll> p_hash, p_pows;
    init(s, p_hash, p_pows);

    while (n--) {
        int a, b, c, d;
        is >> a >> b >> c >> d;
        a--; b--; c--; d--;
        os << (is_equal(s, p_hash, p_pows, a, b, c, d) ? "Yes" : "No") << '\n';
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
