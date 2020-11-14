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

    void move_front(int l, int r) {
        Node *t1 = nullptr, *t2 = nullptr, *t3 = nullptr;
        split(root, r, t2, t3);
        split(t2, l - 1, t1, t2);
        root = merge(t2, t1);
        root = merge(root, t3);
    }

    vector<int> to_vec() const {
        vector<int> res;
        res.reserve(get_size(root));
        to_vec_impl(root, res);
        return res;
    }
};

void solve(istream &is, ostream &os) {
    assert(!dbg);
    int n, m, l, r;
    is >> n >> m;

    TreapArray tarr;
    for (int i = 0; i < n; i++) {
        tarr.add(i, i + 1);
    }

    while (m--) {
        is >> l >> r;
        tarr.move_front(l - 1, r - 1);
    }

    for (int a : tarr.to_vec()) {
        os << a << " ";
    }
    os << endl;
}


int main() {
    // Preps
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);

    return 0;
}
