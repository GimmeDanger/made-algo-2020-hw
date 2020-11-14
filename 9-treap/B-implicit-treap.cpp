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

class TreapArray {
    static constexpr int SUBTREE_INITIAL_SIZE = 1;

    struct Node {
        int priority;
        int value;
        int size;
        Node *left;
        Node *right;

        explicit Node(int value_, int priority_, Node *left_ = nullptr, Node *right_ = nullptr)
                : value(value_), priority(priority_), size(SUBTREE_INITIAL_SIZE), left(left_), right(right_) {}
    };

    // Root of Treap
    Node *root = nullptr;

    int get_size(const Node *node) const {
        return node ? node->size : 0;
    }

    void set_stats(Node *node) {
        if (node) {
            node->size = get_size(node->left) + get_size(node->right) + 1;
        }
    }

    // Complexity: O(h)
    void split(Node *v, int i, Node *&t1, Node *&t2) {
        if (!v) {
            t1 = nullptr;
            t2 = nullptr;
        } else {
            int ls = get_size(v->left);
            if (ls > i) {
                split(v->left, i, t1, t2);
                v->left = t2;
                t2 = v;
                set_stats(t2);
            } else {
                split(v->right, i - ls - 1, t1, t2);
                v->right = t1;
                t1 = v;
                set_stats(t1);
            }
        }
    }

    // Merges trees rooted at t1 and t2 and returns the result as a new tree
    // Invariant required: all keys in t1 must be less than all keys in t2
    // Complexity: O(h)
    Node *merge(Node *t1, Node *t2) {
        if (!t1 || !t2) {
            return (t1 ? t1 : t2);
        } else if (t1->priority > t2->priority) {
            t1->right = merge(t1->right, t2);
            set_stats(t1);
            return t1;
        } else if (t1->priority <= t2->priority) {
            t2->left = merge(t1, t2->left);
            set_stats(t2);
            return t2;
        }
    }

    // Complexity: O(split) + 2 * O(merge) ~ O(h)
    void insert_impl_naive(int i, int val) {
        Node *node = new Node(val, rand());
        Node *t1 = nullptr, *t2 = nullptr;
        split(root, i, t1, t2);
        t1 = merge(t1, node);
        root = merge(t1, t2);
    }

    // Complexity: O(split) + 2 * O(merge) ~ O(h)
    void remove_impl_naive(int i) {
        Node *t1 = nullptr, *t2 = nullptr;
        split(root, i, t1, t2);
        Node *t11 = nullptr, *del_node = nullptr;
        split(t1, i - 1, t11, del_node);
        if (del_node) {
            delete del_node;
            del_node = nullptr;
        }
        root = merge(t11, t2);
    }

    // Complexity: O(h)
    void release_impl(Node *&node) {
        if (node) {
            release_impl(node->left);
            release_impl(node->right);
            delete node;
            node = nullptr;
        }
    }

    // Complexity: O(h)
    void to_vec_impl(Node *node, vector<int> &res) const {
        if (node) {
            to_vec_impl(node->left, res);
            res.push_back(node->value);
            to_vec_impl(node->right, res);
        }
    }

public:
    TreapArray() = default;

    TreapArray(const vector<int> &a) {
        for (int i = 0; i < a.size(); i++) {
            add(i, a[i]);
        }
    }

    ~TreapArray() { release_impl(root); }

    void add(int i, int val) {
        assert(i >= 0 && i <= get_size(root));
        insert_impl_naive(i - 1, val);
    }

    void del(int i) {
        assert(i > 0 && i <= get_size(root));
        remove_impl_naive(i - 1);
    }

    vector<int> to_vec() const {
        vector<int> res;
        res.reserve(get_size(root));
        to_vec_impl(root, res);
        return res;
    }
};

template<typename T>
ostream &operator<<(ostream &os, const optional<T> &val) {
    if (val.has_value()) {
        os << val.value();
    } else {
        os << "none";
    }
    return os;
}

void fuzzy_testing() {
    static constexpr int TESTS = 1000;
    static constexpr int QUERIES = 50;
    static constexpr int MIN_X = 1;
    static constexpr int MAX_X = 50;
    static constexpr int OPS_N = 2;
    std::random_device rand_dev;
    std::mt19937 gen(rand_dev ());
    std::uniform_int_distribution<int> x_distr (MIN_X, MAX_X);
    std::uniform_int_distribution<int> q_distr (0, OPS_N-1);
    for (int t = 0; t < TESTS; t++) {
        vector<tuple<int, int, int>> queries;

        // init array
        const int n = x_distr(gen);
        vector<int> narr(n);
        for (auto &x : narr) {
            x = x_distr(gen);
        }

        // init treap
        TreapArray tarr(narr);

        std::uniform_int_distribution<int> i_distr (1, n * QUERIES);

        for (int q = 0; q < QUERIES; q++) {
            int q_type = q_distr(gen);
            int x = x_distr(gen);
            int i = narr.size() ? 1 + i_distr(gen) % narr.size() : 0;
            queries.push_back({q_type, x, i});
            if (q_type == 0) { //< add
                narr.insert(narr.begin() + i, x);
                tarr.add(i, x);
                assert(narr == tarr.to_vec());
            } else if (q_type == 1 && i > 0) { // del
                auto backup_narr = narr;
                narr.erase(narr.begin() + i - 1);
                auto backup_tarr = tarr.to_vec();
                tarr.del(i);
                auto res_tarr = tarr.to_vec();
                assert(narr == res_tarr);
            }
        }
    }
    cout << "All tests passed!" << endl;
}

void solve(istream &is, ostream &os) {
    assert(!dbg);
    int n, m, a;
    is >> n >> m;
    TreapArray tarr;
    for (int i = 0; i < n; i++) {
        is >> a;
        tarr.add(i, a);
    }
    string command;
    while (m--) {
        is >> command;
        if (command == "add") {
            int i, x;
            is >> i >> x;
            tarr.add(i, x);
        } else if (command == "del") {
            int i;
            is >> i;
            tarr.del(i);
        }
    }
    auto res = tarr.to_vec();
    os << res.size() << endl;
    for (int a : res) {
        os << a << " ";
    }
    os << endl;
}


int main() {
    // Preps
    FAST_IO();
    INPUT_SOURCE();

    if (dbg) {
        fuzzy_testing();
    }
    else {
        solve(cin, cout);
    }

    return 0;
}
