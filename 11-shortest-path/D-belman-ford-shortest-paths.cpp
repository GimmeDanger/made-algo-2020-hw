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
using graph = vector<vector<pair<int, ll>>>;
static constexpr long long INF = numeric_limits<long long>::max() / 2;

void solve(istream &is, ostream &os) {
    ll n, m, s, from, to, w;
    is >> n >> m >> s;
    s--;

    // read graph and store in
    // edge list format
    graph adj(n, vector<pair<int, ll>>());
    for (int i = 0; i < m; i++) {
        is >> from >> to >> w;
        from--; to--;
        adj[from].emplace_back(to, w);
    }

    // run Belman-Ford to find distance from s
    // to every other vertex and some neg cycle vertices
    vector<ll> dist(n, INF);
    vector<int> neg_cycle_candidates;

    dist[s] = 0;
    for (int iter = 1; iter <= n; iter++) {
        bool any_dist_updated = false;
        for (from = 0; from < n; from++) {
            for (auto& e : adj[from]) {
                to = e.first, w = e.second;
                if (dist[from] < INF && dist[to] > dist[from] + w) {
                    any_dist_updated = true;
                    dist[to] = max(-INF, dist[from] + w);
                    if (iter == n) {
                        // relaxed nodes on the last it
                        neg_cycle_candidates.push_back(to);
                    }
                }
            }
        }
        if (!any_dist_updated) {
            break;
        }
    }

    // run multiple source BFS to find all
    // reachable nodes from negative cycle
    vector<bool> visited;
    if (!neg_cycle_candidates.empty ()) {
        visited.resize(n, false);
        queue<int> q;
        for (int v : neg_cycle_candidates) {
            q.push(v);
        }
        while (!q.empty()) {
            from = q.front();
            q.pop();
            for (auto &e : adj[from]) {
                to = e.first;
                if (!visited[to]) {
                    visited[to] = true;
                    q.push(to);
                }
            }
        }
    }

    for (int v = 0; v < n; v++) {
        // check (s, v) path
        bool reachable = false;
        bool shortest = false;
        if (dist[v] < INF) {
            // path exists
            reachable = true;
            if (neg_cycle_candidates.empty() || !visited[v]) {
                // v is not reachable from neg cycle
                // path is shortest (min distance)
                shortest = true;
            }
        }
        // print results
        if (!reachable) {
            os << '*';
        } else if (!shortest) {
            os << '-';
        } else {
            os << dist[v];
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
