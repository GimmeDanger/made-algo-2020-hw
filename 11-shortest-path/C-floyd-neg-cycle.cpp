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

////////////////////// Solver //////////////////////

static constexpr long long INF = numeric_limits<int>::max();
static constexpr int MAX_W = 100000;
static constexpr int INVALID_PREV = -1;

template<typename T>
void print(ostream &os, int n, const vector<vector<T>> &mat) {
    assert(dbg);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (mat[i][j] < INF) {
                os << mat[i][j] << " ";
            } else {
                os << "INF ";
            }
        }
        os << endl;
    }
}

void solve(istream &is, ostream &os) {
    int n, from, to, w;
    is >> n;

    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    vector<vector<int>> next(n, vector<int>(n, INVALID_PREV));
    for (from = 0; from < n; from++) {
        for (to = 0; to < n; to++) {
            is >> w;
            if (w != MAX_W) {
                dist[from][to] = w;
                next[from][to] = to;
            }
        }
    }

    auto get_sum = [](long long d1, long long d2) {
        if (d1 == INF || d2 == INF) {
            return INF;
        }
        // prevent underflowing
        return max<long long>(-INF, d1 + d2);
    };

    // Flow-Warshall algo with path reconstruction
    for (int k = 0; k < n; k++) {
        for (int v = 0; v < n; v++) {
            for (int u = 0; u < n; u++) {
                auto dist_sum = get_sum(dist[v][k], dist[k][u]);
                if (dist[v][u] > dist_sum) {
                    dist[v][u] = dist_sum;
                    next[v][u] = next[v][k];
                }
            }
        }
        if (dbg) {
            print(os, n, dist);
            os << endl;
            print(os, n, next);
            os << endl;
        }
    }

    vector<int> cycle;
    cycle.reserve(n);
    for (int v = 0; v < n; v++) {
        if (dist[v][v] < 0) {
            // prevent inf looping in
            // case of several neg cycles
            int iters = 0;
            // vertex on neg cycle
            cycle.clear();
            cycle.push_back(v);
            for (int u = next[v][v]; u != v && iters < n; u = next[u][v], iters++) {
                cycle.push_back(u);
            }
            if (iters < n) {
                break;
            }
        }
    }

    if (cycle.empty()) {
        os << "NO" << endl;
    } else {
        os << "YES" << endl;
        os << cycle.size() << endl;
        for (int v : cycle) {
            os << v + 1 << " ";
        }
        os << endl;
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
