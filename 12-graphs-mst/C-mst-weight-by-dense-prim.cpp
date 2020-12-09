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

inline double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

long double dense_prim_mst_weight(const vector<int> &x, const vector<int> &y) {

    static constexpr double INF = std::numeric_limits<double>::max();
    static constexpr int INVALID_VERTEX = -1;
    static constexpr int START = 0;
    const int n = x.size();

    vector<bool> used(n, false);
    vector<double> d(n, INF);
    d[START] = 0;

    double mst_weight = 0;
    for (int mst_size = 0; mst_size < n; mst_size++) {

        // find closest vertex
        int next = INVALID_VERTEX;
        for (int v = 0; v < n; v++) {
            if (!used[v] && (next == INVALID_VERTEX || d[v] < d[next]))
                next = v;
        }

        // update mst
        used[next] = true;
        mst_weight += d[next];

        // update closest candidates by adjacent
        for (int u = 0; u < n; u++) {
            d[u] = min(d[u], dist(x[next], y[next], x[u], y[u]));
        }
    }
    return mst_weight;
}

void solve(istream &is, ostream &os) {
    size_t n;
    is >> n;
    vector<int> x(n), y(n);
    for (size_t i = 0; i < n; i++) {
        is >> x[i] >> y[i];
    }
    os << dense_prim_mst_weight(x, y) << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
