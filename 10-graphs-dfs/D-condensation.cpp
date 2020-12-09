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

using graph = vector<unordered_set<int>>;

////////////////////// Toposort implementation //////////////////////

void dfs_topo(const graph &adj, vector<bool> &visited, vector<int> &postorder, int v) {
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u]) {
            dfs_topo(adj, visited, postorder, u);
        }
    }
    postorder.push_back(v);
}

vector<int> toposort(const graph &adj) {

    vector<int> postorder;
    postorder.reserve(adj.size());
    vector<bool> visited(adj.size(), false);

    for (int v = 0; v < adj.size(); v++) {
        if (!visited[v]) {
            dfs_topo(adj, visited, postorder, v);
        }
    }
    reverse(begin(postorder), end(postorder));
    return postorder;
}

////////////////////// Kosaraju–Sharir algorithm implementation //////////////////////

graph reverse(const graph &adj) {
    graph r_adj(adj.size());
    for (int v = 0; v < adj.size(); v++) {
        for (int u : adj[v]) {
            r_adj[u].insert(v);
        }
    }
    return r_adj;
}

static constexpr int INVALID_COMPONENT = -1;

bool not_visited(int c) {
    return c == INVALID_COMPONENT;
}

void dfs_cc(const graph &adj, vector<int> &cc, int cc_curr, int v) {
    cc[v] = cc_curr;
    for (int u : adj[v]) {
        if (not_visited(cc[u])) {
            dfs_cc(adj, cc, cc_curr, u);
        }
    }
}

pair<int, vector<int>> get_strong_components(const graph &adj) {
    int cc_num = 0;
    vector<int> cc(adj.size(), INVALID_COMPONENT);
    for (int v : toposort(reverse(adj))) {
        if (not_visited(cc[v])) {
            dfs_cc(adj, cc, cc_num++, v);
        }
    }
    return {cc_num, cc};
}

////////////////////// Solver //////////////////////

void solve(istream &is, ostream &os) {
    int n, m, u, v;
    is >> n >> m;

    graph adj(n);
    for (int i = 0; i < m; i++) {
        is >> u >> v;
        if (u == v) {
            continue;
        }
        u--; v--;
        adj[u].insert(v);
    }

    auto res = get_strong_components(adj);
    graph cond_adj(res.first);
    auto &cc = res.second;

    size_t edges_in_condensation = 0;
    for (int v = 0; v < n; v++) {
        for (int u : adj[v]) {
            if (cc[v] != cc[u]) {
                auto [it, success] = cond_adj[cc[v]].insert(cc[u]);
                if (success) {
                    edges_in_condensation++;
                }
            }
        }
    }
    os << edges_in_condensation << endl;
}


int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
