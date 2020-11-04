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

class AvlBST {
  struct Node {
    int value;
    int height;
    Node *left;
    Node *right;
    explicit Node(int value_ = 0, int height_ = 1, Node *left_ = nullptr,
                  Node *right_ = nullptr)
        : value(value_), height(height_), left(left_), right(right_) {}
  };
  Node *root = nullptr;

  // Helpers for balancing

  int get_height(const Node *node) const {
    return node ? node->height : 0;
  }

  int get_balance_factor(const Node *node) const {
    return get_height(node->right) - get_height(node->left);
  }

  void set_height(Node *node) {
    if (node) {
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
    set_height(p);
    set_height(q);
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
    set_height(q);
    set_height(p);
    node = p;
  }

  void balance(Node *&node) {
    if (!node) {
      return;
    }
    set_height(node);
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
  ~AvlBST() { release_impl(root); }

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
  for (int t = 0; t < TESTS; t++) {
    vector<pair<int, int>> queries;
    AvlBST bst; NaiveBST nbst;
    for (int q = 0; q < QUERIES; q++) {
      int q_type = rand() % OPS_N;
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
  AvlBST bst;
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
