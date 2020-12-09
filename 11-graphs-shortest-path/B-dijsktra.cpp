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


using edge = pair<int, int>; // {to, weight}
using graph = vector<vector<edge>>;
static constexpr long long INF = numeric_limits<long long>::max();
static constexpr int START = 0;

void solve(istream &is, ostream &os) {
    int n, m, from, to, w;
    is >> n >> m;

    graph adj(n);
    while (m--) {
        is >> from >> to >> w;
        from--; to--;
        adj[from].emplace_back(to, w);
        adj[to].emplace_back(from, w);
    }

    priority_queue<pair<int, int>> pq;
    vector<long long> dist(n, INF);
    vector<bool> visited(n, false);

    dist[START] = 0;
    pq.push({0, START});

    while (!pq.empty()) {
        // get closest vertex
        auto from = pq.top().second;
        pq.pop();

        // mark it as visited
        if (visited[from]) {
            continue;
        }
        visited[from] = true;

        // traverse adjacent
        // to update distances
        for (auto &e : adj[from]) {
            to = e.first, w  = e.second;
            // relax edge if needed
            if (dist[to] > dist[from] + w) {
                // update distances
                dist[to] = dist[from] + w;
                pq.push({-dist[to], to});
            }
        }
    }

    for (int d : dist) {
        os << (d < INF ? to_string(d) : "INF") << " ";
    }
    os << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
