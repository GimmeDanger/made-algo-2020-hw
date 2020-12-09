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

enum class color_t {
    white, //< unvisited
    grey,  //< on stack
    black  //< not on stack
};

void set_on_stack(color_t &c) {
    c = color_t::grey;
}

void set_not_on_stack(color_t &c) {
    c = color_t::black;
}

bool not_visited(color_t c) {
    return c == color_t::white;
}

bool on_stack(color_t c) {
    return c == color_t::grey;
}

void dfs(const graph &adj, vector<color_t> &state, vector<int> &postorder, bool &cycle_detected, int v) {
    if (cycle_detected) {
        return;
    }
    set_on_stack(state[v]);
    for (int u : adj[v]) {
        if (not_visited(state[u])) {
            dfs(adj, state, postorder, cycle_detected, u);
        } else if (on_stack(state[u])) {
            cycle_detected = true;
            return;
        }
    }
    set_not_on_stack(state[v]);
    postorder.push_back(v);
}

vector<int> toposort(const graph &adj) {

    vector<int> postorder;
    postorder.reserve(adj.size());
    vector<color_t> state(adj.size(), color_t::white);

    bool cycle_detected = false;
    for (int v = 0; v < adj.size(); v++) {
        if (not_visited(state[v]) && !cycle_detected) {
            dfs(adj, state, postorder, cycle_detected, v);
        }
    }

    if (cycle_detected) {
        return {};
    } else {
        reverse(begin(postorder), end(postorder));
        return postorder;
    }
}

void solve(istream &is, ostream &os) {
    int n, m, u, v;
    is >> n >> m;

    graph adj(n);
    for (int i = 0; i < m; i++) {
        is >> u >> v;
        u--; v--;
        adj[u].push_back(v);
    }

    auto order = toposort(adj);
    if (order.empty()) {
        os << -1 << endl;
    } else {
        for (int v : order) {
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
