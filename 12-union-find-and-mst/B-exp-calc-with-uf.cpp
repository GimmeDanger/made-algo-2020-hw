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

/**
 В очередной онлайн игре игроки, как обычно, сражаются с монстрами и набирают опыт.
 Для того, чтобы сражаться с монстрами, они объединяются в кланы. После победы над монстром,
 всем участникам клана, победившего его, добавляется одинаковое число единиц опыта.
 Особенностью этой игры является то, что кланы никогда не распадаются и из клана нельзя выйти
 Единственная доступная операция — объединение двух кланов в один.

 Поскольку игроков стало уже много, вам поручили написать систему учета текущего опыта игроков.

 Входные данные:
 В первой строке входного файла содержатся числа n (<= 200000) и m (<= 200000)  — число зарегистрированных игроков и число запросов.
 В следующих m строках содержатся описания запросов. Запросы бывают трех типов:

    join X Y — объединить кланы, в которые входят игроки X и Y (если они уже в одном клане, то ничего не меняется).
    add X V — добавить V единиц опыта всем участникам клана, в который входит игрок X (1 <= V <= 100).
    get X — вывести текущий опыт игрока X.

 Изначально у всех игроков 0 опыта и каждый из них состоит в клане, состоящим из него одного.

 Выходные данные:
 Для каждого запроса get X выведите текущий опыт игрока X.
 */

class UnionFind {
    struct IdStats {
        static constexpr long long INITIAL_SZ = 1;
        static constexpr long long INITIAL_VAL = 0;
        int root;
        int size;
        long long shared_value;

        explicit IdStats(int id_)
                : root(id_),
                  size(INITIAL_SZ),
                  shared_value(INITIAL_VAL) {
        }
    };

    vector<IdStats> id;

    // i -- zero indexed
    int root(int i) {
        while (i != id[i].root) {
            // path compression heuristics
            if (id[i].root != id[id[i].root].root) {
                id[i].shared_value += id[id[i].root].shared_value;
                id[i].root = id[id[i].root].root;
            }
            i = id[i].root;
        }
        return i;
    }

    // i -- zero indexed
    long long path_sum(int i) {
        int r = i;
        // compress path as much as possible
        while (r != id[i].root) {
            r = root(i);
        }
        // ( r ) <---- ( i )
        return id[i].shared_value +
               (i != r ? id[r].shared_value : 0);
    }

public:
    explicit UnionFind(int n) {
        id.reserve(n);
        for (int i = 0; i < n; i++) {
            id.emplace_back(i);
        }
    }

    // i -- zero indexed
    long long get(int i) {
        return path_sum(i);
    }

    // i -- zero indexed
    void add(int i, int v) {
        id[root(i)].shared_value += v;
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
        id[x].shared_value -= id[y].shared_value;
        id[y].size += id[x].size;
    }
};

void solve(istream &is, ostream &os) {
    int n, x, y, v;
    is >> n;
    UnionFind uf(n);
    for (string command; is >> command;) {
        if (command == "join") {
            is >> x >> y;
            x--; y--;
            uf.unite(x, y);
        } else if (command == "get") {
            is >> x;
            x--;
            os << uf.get(x) << '\n';
        } else if (command == "add") {
            is >> x >> v;
            x--;
            uf.add(x, v);
        }
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
