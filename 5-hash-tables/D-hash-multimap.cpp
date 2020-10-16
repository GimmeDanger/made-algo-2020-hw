#include <bits/stdc++.h>
using namespace std;

#define dbg 0

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

class StringHash {
  static constexpr size_t A = 73;
  static constexpr size_t P = 253812577607;
public:
  size_t operator()(const std::string &key) const {
    size_t hash = 0;
    size_t pow = 1;
    for (auto it = rbegin(key); it != rend(key); it++) {
      hash = (hash + pow * (*it)) % P;
      pow = (pow * A) % P;
    }
    return hash;
  }
};

template <typename KeyT, typename KeyHasher>
class HashLinkedSet {

  static constexpr size_t M = 1000;

  struct Node {
    KeyT key;
    Node *prev;
    Node *next;
  };
  using BucketT = std::forward_list<Node>;
  Node *p_last_added = nullptr;
  vector<BucketT> data;
  KeyHasher k_hash;
  size_t size = 0;

private:

  size_t get_bucket_index(const KeyT &key) const {
    return k_hash(key) % M;
  }

public:
  HashLinkedSet()
      : data(vector<BucketT>(M))
  {
  }

  size_t get_size() const {
    return size;
  }

  void put(KeyT key) {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](Node &node) {
      return node.key == key;
    });
    if (it != end(bucket)) {
      // update exising node
      it->key = std::move(key);
    }
    else {
      // add new node
      bucket.push_front({std::move(key), p_last_added, nullptr});
      auto &curr_node = *begin(bucket);
      if (p_last_added) {
        p_last_added->next = &curr_node;
      }
      p_last_added = &curr_node;
      size++;
    }
  }

  void remove(KeyT key) {
    auto &bucket = data[get_bucket_index(key)];
    bucket.remove_if([this, &key](Node &node) {
      if (node.key == key) {
        if (node.prev) {
          // update prev before removing
          node.prev->next = node.next;
        }
        if (node.next) {
          // update next before removing
          node.next->prev = node.prev;
        }
        if (&node == p_last_added) {
          p_last_added = node.prev;
        }
        size--;
        return true;
      }
      return false;
    });
  }

  bool contains(const KeyT &key) const {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](const Node &node) {
      return node.key == key;
    });
    return it != end(bucket);
  }

  void print_all_elements() const {
    for (Node *curr = p_last_added; curr != nullptr; curr = curr->prev) {
      std::cout << curr->key << " ";
    }
  }
};

template <typename KeyT, typename ValT, typename KeyHasher, typename ValHasher>
class MultiHashMap {
  static constexpr int M = 100'000;
  using NodeKeyT = KeyT;
  using NodeValT = HashLinkedSet<ValT, ValHasher>;
  using Node = std::pair<NodeKeyT, NodeValT>;
  using BucketT = forward_list<Node>;
  vector<BucketT> data;
  KeyHasher k_hash;

private:
  size_t get_bucket_index(const KeyT &key) const {
    return k_hash(key) % M;
  }

public:
  MultiHashMap()
    : data(vector<BucketT>(M))
  {
  }

  /**
   * Adds pair {key, value}, if element is not presented
   */
  void put(KeyT key, ValT val) {
    auto &bucket = data[get_bucket_index(key)];
    auto it = find_if(begin(bucket), end(bucket), [&key](auto &node) {
      return node.first == key;
    });
    if (it == end(bucket)) {
      bucket.push_front({key, NodeValT()});
      it = begin(bucket);
    }
    it->second.put(std::move(val));
  }

  /**
   * Prints number of pair with key x, and a list of their values
   * Format: <N val_1 ... val_N>
   */
  void get(KeyT key) const {
    auto &bucket = data[get_bucket_index(key)];
    auto it = find_if(begin(bucket), end(bucket), [&key](auto &node) {
      return node.first == key;
    });
    if (it == end(bucket)) {
      std::cout << "0\n";
    }
    else {
      auto &value_map = it->second;
      std::cout << value_map.get_size() << " ";
      if (value_map.get_size() > 0)
        value_map.print_all_elements();
      std::cout << "\n";
    }
  }

  /**
   * Deletes pair {key, val}, if element is presented
   */
  void remove(KeyT key, ValT val) {
    auto &bucket = data[get_bucket_index(key)];
    auto it = find_if(begin(bucket), end(bucket), [&key](auto &node) {
      return node.first == key;
    });
    if (it != end(bucket)) {
      auto &value_map = it->second;
      value_map.remove(std::move(val));
      if (value_map.get_size() == 0) {
        bucket.remove_if([&key](auto &node) {
          return node.first == key;
        });
      }
    }
  }

  /**
   * Deletes all pairs {key, *}
   */
  void removeAll(KeyT key) {
    auto &bucket = data[get_bucket_index(key)];
    bucket.remove_if([&key](auto &node) {
      return node.first == key;
    });
  }
};

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  MultiHashMap<string, string, StringHash, StringHash> mp;

  string command;
  while (cin >> command) {
    if (command == "put") {
      string x, y;
      cin >> x >> y;
      mp.put(move(x), move(y));
    }
    else if (command == "get") {
      string x;
      cin >> x;
      mp.get(move(x));
    }
    else if (command == "delete") {
      string x, y;
      cin >> x >> y;
      mp.remove(move(x), move(y));
    }
    else if (command == "deleteall") {
      string x;
      cin >> x;
      mp.removeAll(move(x));
    }
    else {
      assert(0);
    }
  }
  return 0;
}
