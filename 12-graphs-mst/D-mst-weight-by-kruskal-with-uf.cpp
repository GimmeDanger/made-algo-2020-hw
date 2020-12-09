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

class UnionFind {
    struct IdStats {
        static constexpr int INITIAL_SZ = 1;
        int root;
        int size;

        explicit IdStats(int id_)
                : root(id_), size(INITIAL_SZ) {
        }
    };

    vector<IdStats> id;

    // i -- zero indexed
    int root(int i) {
        while (i != id[i].root) {
            // path compression heuristics
            id[i].root = id[id[i].root].root;
            i = id[i].root;
        }
        return i;
    }

public:
    explicit UnionFind(int n) {
        id.reserve(n);
        for (int i = 0; i < n; i++) {
            id.emplace_back(i);
        }
    }

    bool is_connected(int x, int y) {
        return root(x) == root(y);
    }

    // x, y -- zero based
    void unite(int x, int y) {
        x = root(x);
        y = root(y);
        if (x == y) {
            return;
        }
        // weighting heuristics
        if (id[x].size > id[x].size) {
            swap(x, y);
        }
        id[x].root = y;
        id[y].size += id[x].size;
    }
};

struct edge {
    int b;
    int e;
    int w;
};

void solve(istream &is, ostream &os) {
    int n, m;
    is >> n >> m;
    vector<edge> edges(m);
    for (int i = 0; i < m; i++) {
        is >> edges[i].b >> edges[i].e >> edges[i].w;
        edges[i].b--; edges[i].e--;
    }
    // kruskal algorithm
    // initialization
    UnionFind uf(n);
    sort(begin(edges), end(edges), [](auto& lhs, auto& rhs) {
       return lhs.w < rhs.w;
    });
    // mst reconstruction
    size_t mst_size = 0;
    long long mst_weight = 0;
    for (auto& e : edges) {
        if (mst_size == n - 1) {
            break;
        }
        if (!uf.is_connected(e.b, e.e)) {
            mst_size++;
            mst_weight += e.w;
            uf.unite(e.b, e.e);
        }
    }
    assert(mst_size == n - 1);
    os << mst_weight << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
