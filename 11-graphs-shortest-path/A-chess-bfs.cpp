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

using vertex = pair<int, int>;
static constexpr vertex INVALID_VERTEX = {-1, -1};
static constexpr array<vertex, 8> deltas = {
        vertex{+1, +2},
        vertex{-1, +2},
        vertex{+2, +1},
        vertex{+2, -1},
        vertex{+1, -2},
        vertex{-1, -2},
        vertex{-2, +1},
        vertex{-2, -1}
};

void solve(istream &is, ostream &os) {
    int n, s_x, s_y, t_x, t_y;
    is >> n >> s_x >> s_y >> t_x >> t_y;
    s_x--; s_y--; t_x--; t_y--;

    queue<vertex> q;
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    vector<vector<vertex>> prev(n, vector<vertex>(n, INVALID_VERTEX));

    auto is_valid = [n](int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < n;
    };

    q.push({s_x, s_y});

    while (!q.empty()) {
        auto v = q.front(); q.pop();
        visited[v.first][v.second] = true;
        for (auto &d : deltas) {
            int v_x = v.first + d.first;
            int v_y = v.second + d.second;
            if (is_valid(v_x, v_y) && !visited[v_x][v_y]) {
                q.push({v_x, v_y});
                prev[v_x][v_y] = {v.first, v.second};
            }
        }
    }

    stack<vertex> path;
    path.push({t_x, t_y});
    while (prev[path.top().first][path.top().second] != INVALID_VERTEX) {
        path.push(prev[path.top().first][path.top().second]);
    }
    os << path.size() << endl;
    while (!path.empty()) {
        os << path.top().first + 1 << " "
           << path.top().second + 1 << endl;
        path.pop();
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
