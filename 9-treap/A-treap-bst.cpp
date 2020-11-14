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

template<bool use_naive_update_strategy = false>
class TreapBST {
    struct Node {
        int x;
        int y;
        Node *left;
        Node *right;
        explicit Node(int x_, int y_, Node *left_ = nullptr, Node *right_ = nullptr)
                : x(x_), y(y_), left(left_), right(right_) {}
    };
    Node *root = nullptr;

    // Splits the tree rooted at v by x key into trees rooted at t1 and t2
    // Complexity: O(h)
    void split(Node *v, int x, Node *&t1, Node *&t2) {
        if (!v) {
            t1 = nullptr;
            t2 = nullptr;
        } else if (v->x > x) {
            split(v->left, x, t1, t2);
            v->left = t2;
            t2 = v;
        } else if (v->x <= x) {
            split(v->right, x, t1, t2);
            v->right = t1;
            t1 = v;
        }
    }

    // Merges trees rooted at t1 and t2 and returns the result as a new tree
    // Invariant required: all keys in t1 must be less than all keys in t2
    // Complexity: O(h)
    Node *merge(Node *t1, Node *t2) {
        if (!t1 || !t2) {
            return (t1 ? t1 : t2);
        } else if (t1->y > t2->y) {
            t1->right = merge(t1->right, t2);
            return t1;
        } else if (t1->y <= t2->y) {
            t2->left = merge(t1, t2->left);
            return t2;
        }
    }

    // Inserts new node (x, y),
    // Invariant: node must not exist in tree,
    // Complexity: O(split) + 2 * O(merge) ~ O(h)
    void insert_impl_naive(int x, int y) {
        Node *node = new Node(x, y);
        Node *t1 = nullptr, *t2 = nullptr;
        split(root, x, t1, t2);
        t1 = merge(t1, node);
        root = merge(t1, t2);
    }

    // Inserts new node (x, y),
    // Invariant: node must not exist in tree,
    // Complexity: O(split) ~ O(h)
    void insert_impl(Node *&node, int x, int y) {
        if (!node || node->y < y) {
            Node *new_node = new Node(x, y);
            if (node) {
                Node *t1 = nullptr, *t2 = nullptr;
                split(node, x, t1, t2);
                new_node->left = t1;
                new_node->right = t2;
            }
            node = new_node;
        } else if (node->x > x) {
            insert_impl(node->left, x, y);
        } else if (node->x < x) {
            insert_impl(node->right, x, y);
        }
    }

    // Deletes node (x),
    // Invariant: node must exist in tree
    // Complexity: O(split) + 2 * O(merge) ~ O(h)
    void remove_impl_naive(int x) {
        Node *t1 = nullptr, *t2 = nullptr;
        split(root, x, t1, t2);
        Node *t11 = nullptr, *del_node = nullptr;
        split(t1, x - 1, t11, del_node);
        if (del_node) {
            delete del_node;
            del_node = nullptr;
        }
        root = merge(t11, t2);
    }

    // Deletes node (x),
    // Invariant: node must exist in tree
    // Complexity: O(split) ~ O(h)
    void remove_impl(Node *&node, int x) {
        if (!node) {
            return;
        }
        if (node->x > x) {
            remove_impl(node->left, x);
        } else if (node->x < x) {
            remove_impl(node->right, x);
        } else {
            auto *del_node = node;
            node = merge(node->left, node->right);
            delete del_node;
            del_node = nullptr;
        }
    }

    // Complexity: O(h)
    void exists_impl(const Node *node, int x, bool &res) const {
        if (node) {
            if (node->x > x) {
                exists_impl(node->left, x, res);
            } else if (node->x < x) {
                exists_impl(node->right, x, res);
            } else {
                res = true;
            }
        }
    }

    // Complexity: O(h)
    void next_impl(const Node *node, int x, optional<int> &res) const {
        if (node) {
            if (node->x > x) {
                res = node->x;
                next_impl(node->left, x, res);
            } else {
                next_impl(node->right, x, res);
            }
        }
    }

    // Complexity: O(h)
    void prev_impl(const Node *node, int x, optional<int> &res) const {
        if (node) {
            if (node->x >= x) {
                prev_impl(node->left, x, res);
            } else {
                res = node->x;
                prev_impl(node->right, x, res);
            }
        }
    }

    // Complexity: O(h)
    void print_impl(const Node *node, int space) const {
        static constexpr int LEVEL_DISTANCE = 5;
        if (node) {
            space += LEVEL_DISTANCE;
            print_impl(node->right, space);

            cout << "\n";
            for (int i = LEVEL_DISTANCE; i < space; i++) {
                cout << " ";
            }
            cout << node->x << "\n";
            print_impl(node->left, space);
        }
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

public:
    // All constructors are default
    ~TreapBST() { release_impl(root); }

    void insert(int x) {
        if (!exists(x)) {
            if (use_naive_update_strategy) {
                insert_impl_naive(x, rand());
            } else {
                insert_impl(root, x, rand());
            }
        }
    }

    void remove(int x) {
        if (exists(x)) {
            if (use_naive_update_strategy) {
                remove_impl_naive(x);
            } else {
                remove_impl(root, x);
            }
        }
    }

    bool exists(int x) const {
        bool res = false;
        exists_impl(root, x, res);
        return res;
    }

    optional<int> next(int x) const {
        optional<int> res;
        next_impl(root, x, res);
        return res;
    }

    optional<int> prev(int x) const {
        optional<int> res;
        prev_impl(root, x, res);
        return res;
    }

    void print() const {
        cout << "--BST--\n";
        print_impl(root, 0);
        cout << "-------\n\n";
    }
};

class NaiveBST {
    struct Node {
        int value;
        Node *left;
        Node *right;
        explicit Node(int value_ = 0, Node *left_ = nullptr, Node *right_ = nullptr)
                : value(value_), left(left_), right(right_) {}
    };
    Node *root = nullptr;

    void insert_impl(Node *&node, int x) {
        if (node) {
            if (node->value > x) {
                insert_impl(node->left, x);
            } else if (node->value < x) {
                insert_impl(node->right, x);
            }
        } else {
            node = new Node(x);
        }
    }

    void remove_impl(Node *&node, int x) {
        if (node) {
            if (node->value > x) {
                remove_impl(node->left, x);
            } else if (node->value < x) {
                remove_impl(node->right, x);
            } else {
                auto *left = node->left;
                auto *right = node->right;
                delete node;
                if (left) {
                    node = left;
                    auto *largest = left;
                    while (largest->right) {
                        largest = largest->right;
                    }
                    largest->right = right;
                } else {
                    node = right;
                }
            }
        }
    }

    void exists_impl(const Node *node, int x, bool &res) const {
        if (node) {
            if (node->value > x) {
                exists_impl(node->left, x, res);
            } else if (node->value < x) {
                exists_impl(node->right, x, res);
            } else {
                res = true;
            }
        }
    }

    void next_impl(const Node *node, int x, optional<int> &res) const {
        if (node) {
            if (node->value > x) {
                res = node->value;
                next_impl(node->left, x, res);
            } else {
                next_impl(node->right, x, res);
            }
        }
    }

    void prev_impl(const Node *node, int x, optional<int> &res) const {
        if (node) {
            if (node->value >= x) {
                prev_impl(node->left, x, res);
            } else {
                res = node->value;
                prev_impl(node->right, x, res);
            }
        }
    }

    void print_impl(const Node *node, int space) const {
        static constexpr int LEVEL_DISTANCE = 5;
        if (node) {
            space += LEVEL_DISTANCE;
            print_impl(node->right, space);

            cout << "\n";
            for (int i = LEVEL_DISTANCE; i < space; i++) {
                cout << " ";
            }
            cout << node->value << "\n";
            print_impl(node->left, space);
        }
    }

    void release_impl(Node *&node) {
        if (node) {
            release_impl(node->left);
            release_impl(node->right);
            delete node;
            node = nullptr;
        }
    }

public:
    // All constructors are default
    ~NaiveBST() { release_impl(root); }

    void insert(int x) { insert_impl(root, x); }
    void remove(int x) { remove_impl(root, x); }

    bool exists(int x) const {
        bool res = false;
        exists_impl(root, x, res);
        return res;
    }

    optional<int> next(int x) const {
        optional<int> res;
        next_impl(root, x, res);
        return res;
    }

    optional<int> prev(int x) const {
        optional<int> res;
        prev_impl(root, x, res);
        return res;
    }

    void print() const {
        cout << "--BST--\n";
        print_impl(root, 0);
        cout << "-------\n\n";
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
    static constexpr int OPS_N = 5;
    std::random_device rand_dev;
    std::mt19937 gen(rand_dev ());
    std::uniform_int_distribution<int> x_distr (MIN_X, MAX_X);
    std::uniform_int_distribution<int> q_distr (0, OPS_N-1);
    for (int t = 0; t < TESTS; t++) {
        vector<pair<int, int>> queries;
        TreapBST bst; NaiveBST nbst;
        for (int q = 0; q < QUERIES; q++) {
            int q_type = q_distr(gen);
            int x = x_distr(gen);
            queries.push_back({q_type, x});
            if (q_type == 0) {
                bst.insert(x);
                nbst.insert(x);
            } else if (q_type == 1) {
                bst.remove(x);
                nbst.remove(x);
            } else if (q_type == 2) {
                auto res1 = bst.exists(x);
                auto res2 = nbst.exists(x);
                assert(res1 == res2);
            } else if (q_type == 3) {
                auto res1 = bst.next(x);
                auto res2 = nbst.next(x);
                assert(res1 == res2);
            } else if (q_type == 4) {
                auto res1 = bst.prev(x);
                auto res2 = nbst.prev(x);
                assert(res1 == res2);
            }
        }
    }
    cout << "All tests passed!" << endl;
}

void solve(istream &is, ostream &os) {
    assert(!dbg);
    TreapBST bst;
    for (string command; is >> command;) {
        int x;
        is >> x;
        if (command == "insert") {
            bst.insert(x);
        } else if (command == "delete") {
            bst.remove(x);
        } else if (command == "exists") {
            os << (bst.exists(x) ? "true" : "false") << endl;
        } else if (command == "next") {
            os << bst.next(x) << endl;
        } else if (command == "prev") {
            os << bst.prev(x) << endl;
        }
    }
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
