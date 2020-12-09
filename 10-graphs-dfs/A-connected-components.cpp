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

using graph = vector<vector<int>>;

static constexpr int INVALID_COLOR = 0;

static bool not_visited(int c) {
    return c == INVALID_COLOR;
}

void dfs(const graph &adj, vector<int> &cc, int v, int c) {
    cc[v] = c;
    for (int u : adj[v]) {
        if (not_visited(cc[u])) {
            dfs(adj, cc, u, c);
        }
    }
}

pair<int, vector<int>> get_connected_components(const graph &adj) {
    int c_num = INVALID_COLOR;
    vector<int> cc(adj.size(), INVALID_COLOR);
    for (int v = 0; v < adj.size(); v++) {
        if (not_visited(cc[v])) {
            dfs(adj, cc, v, ++c_num);
        }
    }
    return {c_num, cc};
}

void solve(istream &is, ostream &os) {
    int n, m, u, v;
    is >> n >> m;
    graph adj(n);
    for (int i = 0; i < m; i++) {
        is >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    auto cc = get_connected_components(adj);
    os << cc.first << endl;
    for (int a : cc.second) {
        os << a << " ";
    }
    os << endl;
}


int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
