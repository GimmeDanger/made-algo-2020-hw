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

int dfs(const graph &adj, vector<bool> &visited, int v) {
    int res = 0;
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u]) {
            res = max(res, dfs(adj, visited, u));
        }
    }
    return res + 1;
}

int get_longest_path_len(const graph &adj) {
    int res = 0;
    vector<bool> visited(adj.size());
    for (int v = 0; v < adj.size(); v++) {
        fill(begin(visited), end(visited), false);
        res = max(res, dfs(adj, visited, v));
    }
    return res;
}

// insert string in mapping if needed,
// returns corresponding vertex id
int get_vertex_id(map<string, int> &name_to_vertex, string &s) {
    transform(s.begin(), s.end(), s.begin(), [](char ch) {
        return tolower(ch);
    });
    auto it = name_to_vertex.find(s);
    if (it != name_to_vertex.end()) {
        return it->second;
    }
    int vertex_id = name_to_vertex.size();
    name_to_vertex[s] = vertex_id;
    return vertex_id;
}

void solve(istream &is, ostream &os) {
    int n;
    is >> n;
    string u, t, v;

    map<string, int> name_to_vertex;
    vector<pair<int, int>> edges;
    for (int i = 0; i < n; i++) {
        is >> u >> t >> v;
        int u_id = get_vertex_id(name_to_vertex, u);
        int v_id = get_vertex_id(name_to_vertex, v);
        edges.push_back({v_id, u_id});
    }
    graph adj(name_to_vertex.size());
    for (auto &e : edges) {
        adj[e.first].push_back(e.second);
    }
    os << get_longest_path_len(adj) << endl;
}


int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
