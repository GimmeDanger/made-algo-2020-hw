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

static constexpr int NETWORK_EDGES_PER_INPUT_UNDIRECT_EDGE = 4;
static constexpr int NETWORK_EDGES_PER_INPUT_DIRECT_EDGE = 2;
static constexpr int INF = numeric_limits<int>::max() / 2;
static constexpr int INVALID_ID = -1;

vector<bool> compute_reachable(const flow_network &graph, int s) {
    vector<bool> used(graph.size(), false);
    used[s] = true;

    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int id : graph.adj(v)) {
            auto &e = graph.get_edge(id);
            int w = e.other(v);
            if (!used[w] && e.residual_capacity() > 0) {
                used[w] = true;
                q.push(w);
            }
        }
    }
    return used;
}

bool has_path(flow_network &graph, vector<int> &edge_to, vector<bool> &used, int s, int t, int scale) {
    used.assign(graph.size(), false);
    edge_to.assign(graph.size(), INVALID_ID);
    queue<int> q;

    used[s] = true;
    q.push(s);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        if (v == t) {
            break;
        }
        for (int id : graph.adj(v)) {
            auto &e = graph.get_edge(id);
            int w = e.other(v);
            if (!used[w] && e.residual_capacity() >= scale) {
                edge_to[w] = id;
                used[w] = true;
                q.push(w);
            }
        }
    }
    return used[t];
}


long long edmonds_karp_max_flow(flow_network &graph, int s, int t) {
    long long max_flow = 0;
    vector<bool> used(graph.size());
    vector<int> edge_to(graph.size());
    // compute max scale
    int max_capacity = -INF;
    for (int v = 0; v < graph.size(); v++) {
        for (int id : graph.adj(v)) {
            auto &e = graph.get_edge(id);
            max_capacity = max(max_capacity, e.capacity());
        }
    }
    unsigned int scale = 1;
    while (scale < max_capacity) {
        scale <<= 1;
    }
    while (scale >= 1) {
        while (has_path(graph, edge_to, used, s, t, scale)) {
            int bottleneck = INF;
            // compute bottleneck capacity
            for (int v = t; v != s; ) {
                auto id = edge_to[v];
                auto &e = graph.get_edge(id);
                bottleneck = min(bottleneck, e.residual_capacity());
                v = e.other(v);
            }
            // update augment flow
            for (int v = t; v != s; ) {
                auto id = edge_to[v];
                auto &e = graph.get_edge(id);
                graph.update_flow(id, bottleneck);
                v = e.other(v);
            }
            max_flow += bottleneck;
        }
        scale >>= 1;
    }
    return max_flow;
}


void solve(istream &is, ostream &os) {
    int n, m;
    is >> n >> m;
    flow_network graph(n);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        is >> a >> b >> c;
        graph.add_edge(a - 1, b - 1, c);
        graph.add_edge(b - 1, a - 1, c);
    }

    // find max flow
    int s = 0, t = n - 1;
    int max_flow = edmonds_karp_max_flow(graph, s, t);

    // find min cut properties
    vector<int> min_cut_edges;
    auto reachable_from_s = compute_reachable(graph, s);
    auto get_direct_edge = [&graph](int input_id) -> const flow_edge & {
        int graph_id = input_id * NETWORK_EDGES_PER_INPUT_UNDIRECT_EDGE;
        return graph.get_edge(graph_id);
    };
    for (int i = 0; i < m; i++) {
        const auto &e = get_direct_edge(i);
        if (reachable_from_s[e.from()] != reachable_from_s[e.to()]) {
            min_cut_edges.push_back(i);
        }
    }
    os << min_cut_edges.size() << " " << max_flow << endl;
    for (int i : min_cut_edges) {
        os << i + 1 << " ";
    }
    os << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
