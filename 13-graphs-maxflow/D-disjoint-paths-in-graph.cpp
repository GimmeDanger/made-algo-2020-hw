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

class flow_edge {
private:
    const int m_v;
    const int m_w;
    const int m_capacity;
    int m_flow = 0;

public:
    explicit flow_edge(int v, int w, int capacity) :
            m_v(v), m_w(w), m_capacity(capacity), m_flow(0) {}

    int from() const { return m_v; }

    int to() const { return m_w; }

    int capacity() const { return m_capacity; }

    int flow() const { return m_flow; }

    int residual_capacity() const {
        return m_capacity - m_flow;
    }

    int other(int vertex) const {
        if (vertex == m_v) {
            return m_w;
        } else if (vertex == m_w) {
            return m_v;
        } else {
            assert (0);
            return -1;
        }
    }

    void add_flow(int delta) {
        m_flow += delta;
    }

    void sub_flow(int delta) {
        m_flow -= delta;
    }
};


class flow_network {
private:
    const int v;
    vector<flow_edge> edges;
    vector<vector<int>> graph;

public:
    explicit flow_network(int n) : v(n), graph(n) {}

    void add_edge(int from, int to, int capacity) {
        graph[from].emplace_back(edges.size());
        edges.emplace_back(from, to, capacity);
        graph[to].emplace_back(edges.size());
        edges.emplace_back(from, to, 0);
    }

    int size() const {
        return v;
    }

    const vector<int> &adj(int v) const {
        return graph[v];
    }

    vector<int> &adj(int v) {
        return graph[v];
    }

    const flow_edge &get_edge(int id) const {
        return edges[id];
    }

    flow_edge &get_edge(int id) {
        return edges[id];
    }

    void update_flow(int id, int delta) {
        edges[id].add_flow(delta);
        edges[id ^ 1].sub_flow(delta);
    }
};

int push_flow(flow_network &graph, vector<bool> &used, int s, int t, int curr_flow) {
    used[s] = true;
    if (s == t) {
        return curr_flow;
    }
    for (int id : graph.adj(s)) {
        auto &e = graph.get_edge(id);
        int w = e.other(s);
        if (!used[w] && e.residual_capacity() > 0) {
            auto next_flow = min(curr_flow, e.residual_capacity());
            auto delta = push_flow(graph, used, w, t, next_flow);
            if (delta > 0) {
                graph.update_flow(id, delta);
                return delta;
            }
        }
    }
    return 0;
}


long long ford_fulkerson_max_flow(flow_network &graph, int s, int t) {
    static constexpr int INF = numeric_limits<int>::max();
    long long value = 0;
    vector<bool> used(graph.size());
    while (true) {
        used.assign(graph.size(), false);
        auto bottleneck = push_flow(graph, used, s, t, INF);
        if (bottleneck > 0) {
            value += bottleneck;
        } else {
            break;
        }
    }
    return value;
}

vector<int> occupy_saturated_path(flow_network &graph, int s, int t) {
    static constexpr int SATURATED_FLOW = 1;
    static constexpr int INVALID_VERTEX = -1;
    vector<int> path;
    for (int v = s; v != t; ) {
        int next = -1;
        for (int id : graph.adj(v)) {
            auto &e = graph.get_edge(id);
            if (e.flow() == SATURATED_FLOW) {
                path.push_back(v);
                e.sub_flow(e.flow());
                next = e.other(v);
                break;
            }
        }
        if (next == INVALID_VERTEX) {
            throw logic_error("saturated edge must exist");
        }
        v = next;
    }
    path.push_back(t);
    return path;
}

// Find two disjoint paths from <s> to <t> in directed graph,
// Run FF max flow lookup in original graph with capacities equal to 1 (stage 1),
// If max flow >= REQUIRED_PATHS, we pick any saturated paths, such that edges on them has flow = 1 (stage 2)
void solve(istream &is, ostream &os) {
    static constexpr int INITIAL_CAPACITY = 1;
    static constexpr int REQUIRED_PATHS = 2;

    int n, m, s, t, x, y;
    is >> n >> m >> s >> t;
    s--; t--;
    flow_network graph(n);
    while (m--) {
        is >> x >> y;
        x--; y--;
        graph.add_edge(x, y, INITIAL_CAPACITY);
    }

    if (ford_fulkerson_max_flow(graph, s, t) >= REQUIRED_PATHS) {
        os << "YES" << endl;
        for (int i = 0; i < REQUIRED_PATHS; i++) {
            for (int v : occupy_saturated_path(graph, s, t)) {
                os << v + 1 << " ";
            }
            os << endl;
        }
    } else {
        os << "NO" << endl;
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}