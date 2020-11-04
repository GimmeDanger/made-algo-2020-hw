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

class AvlKMaxBST {
  struct Node {
    static constexpr int SIZE_DEFAULT = 1;
    static constexpr int HEIGHT_DEFAULT = 1;
    int value;  //< value in node
    int size;   //< number of elements in subtree rooted at node, needed for k-max
    int height; //< height of subtree rooted at node, needed for balancing
    Node *left;
    Node *right;
    explicit Node(int value_ = 0, Node *left_ = nullptr, Node *right_ = nullptr)
        : value(value_),
          size(SIZE_DEFAULT),
          height(HEIGHT_DEFAULT),
          left(left_),
          right(right_) {}
  };

  Node *root = nullptr;

  // Helpers for balancing

  int get_height(const Node *node) const {
    return node ? node->height : 0;
  }

  int get_size(const Node *node) const {
    return node ? node->size : 0;
  }

  int get_balance_factor(const Node *node) const {
    return get_height(node->right) - get_height(node->left);
  }

  void set_stats(Node *node) {
    if (node) {
      node->size = get_size(node->left) + get_size(node->right) + 1;
      node->height = max(get_height(node->left), get_height(node->right)) + 1;
    }
  }

  /*            (p)     =>         (q)        (p)      =>       (q)
   *           /   \    =>        /   \      /   \     =>      /   \
   *        (q)         =>                 (m)         =>         (p)
   *       /   \        =>                             =>        /   \
   *            (m)     =>                             =>       (m)
   */
  void rotate_right(Node *&node) {
    auto *p = node;
    auto *q = node->left;
    auto *m = q->right;
    p->left = m;
    q->right = p;
    set_stats(p);
    set_stats(q);
    node = q;
  }

  /*            (p)     <=         (q)        (p)      <=       (q)
   *           /   \    <=        /   \      /   \     <=      /   \
   *        (q)         <=                 (m)         <=         (p)
   *       /   \        <=                             <=        /   \
   *            (m)     <=                             <=       (m)
   */
  void rotate_left(Node *&node) {
    auto *q = node;
    auto *p = node->right;
    auto *m = p->left;
    q->right = m;
    p->left = q;
    set_stats(q);
    set_stats(p);
    node = p;
  }

  void balance(Node *&node) {
    if (!node) {
      return;
    }
    set_stats(node);
    if (get_balance_factor(node) == 2) {
      if (get_balance_factor(node->right) < 0) {
        rotate_right(node->right);
      }
      rotate_left(node);
    }
    if (get_balance_factor(node) == -2) {
      if (get_balance_factor(node->left) > 0) {
        rotate_left(node->left);
      }
      rotate_right(node);
    }
  }

  void remove_smallest(Node *&node, int &res) {
    if (!node) {
      return;
    }
    if (node->left) {
      remove_smallest(node->left, res);
    } else {
      res = node->value;
      auto *right = node->right;
      delete node;
      node = right;
    }
    balance(node);
  }

  void insert_impl(Node *&node, int x) {
    if (!node) {
      node = new Node(x);
    }
    if (node->value > x) {
      insert_impl(node->left, x);
    } else if (node->value < x) {
      insert_impl(node->right, x);
    }
    balance(node);
  }

  void remove_impl(Node *&node, int x) {
    if (!node) {
      return;
    }
    if (node->value > x) {
      remove_impl(node->left, x);
    } else if (node->value < x) {
      remove_impl(node->right, x);
    } else {
      auto *left = node->left;
      auto *right = node->right;
      if (right) {
        int smallest_value = right->value;
        remove_smallest(right, smallest_value);
        node->value = smallest_value;
        node->right = right;
      } else {
        delete node;
        node = left;
      }
    }
    balance(node);
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

  // returns k-th minimum or nullopt, k indexed from 0
  void k_min_impl(const Node *node, int k, optional<int> &res) const {
    if (node) {
      int lw = get_size(node->left);
      if (lw > k) {
        k_min_impl(node->left, k, res);
      } else if (lw < k) {
        k_min_impl(node->right, k - lw - 1, res);
      } else {
        res = node->value;
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
  ~AvlKMaxBST() { release_impl(root); }

  void insert(int x) { insert_impl(root, x); }
  void remove(int x) { remove_impl(root, x); }

  bool exists(int x) const {
    bool res = false;
    exists_impl(root, x, res);
    return res;
  }

  // returns k-th maximum or nullopt,
  // k indexed from 0
  optional<int> k_max(int k) const {
    optional<int> res;
    k_min_impl(root, get_size(root) - (k + 1), res);
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

void solve(istream &is, ostream &os) {
  int n;
  is >> n;
  AvlKMaxBST bst;
  while (n--) {
    int c, x;
    is >> c >> x;
    if (c > 0) {
      bst.insert(x);
    } else if (c < 0) {
      bst.remove(x);
    } else if (c == 0) {
      os << bst.k_max(x-1) << endl;
    }
  }
}


int main() {
  FAST_IO();
  INPUT_SOURCE();
  solve(cin, cout);
  return 0;
}
