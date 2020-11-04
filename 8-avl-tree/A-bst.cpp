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

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  NaiveBST bst;
  for (string command; cin >> command;) {
    int x;
    cin >> x;
    if (command == "insert") {
      bst.insert(x);
    } else if (command == "delete") {
      bst.remove(x);
    } else if (command == "exists") {
      cout << (bst.exists(x) ? "true" : "false") << "\n";
    } else if (command == "next") {
      cout << bst.next(x) << "\n";
    } else if (command == "prev") {
      cout << bst.prev(x) << "\n";
    }
    if (dbg) {
      bst.print();
    }
  }

  return 0;
}
