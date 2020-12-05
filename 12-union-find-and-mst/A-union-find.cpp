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
        int min;
        int max;
        int size;

        explicit IdStats(int id_)
                : root(id_), min(id_), max(id_), size(INITIAL_SZ) {
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

    // i -- zero indexed
    const IdStats &get(int i) {
        // only root statistics has correct values
        return id[root(i)];
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
        id[y].min = std::min(id[x].min, id[y].min);
        id[y].max = std::max(id[x].max, id[y].max);
    }
};

void solve(istream &is, ostream &os) {
    int n, x, y;
    is >> n;
    UnionFind uf(n);
    for (string command; is >> command;) {
        if (command == "union") {
            is >> x >> y;
            x--;
            y--;
            uf.unite(x, y);
        } else if (command == "get") {
            is >> x;
            x--;
            auto &stats = uf.get(x);
            os << stats.min + 1 << " "
               << stats.max + 1 << " "
               << stats.size << endl;
        }
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
