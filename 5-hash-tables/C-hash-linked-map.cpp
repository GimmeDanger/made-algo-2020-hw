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

template <typename KeyT, typename ValueT, typename KeyHasher>
class HashLinkedMap {

  static constexpr size_t M = 100'000;

  struct Node {
    KeyT key;
    ValueT val;
    Node *prev;
    Node *next;
  };
  using BucketT = std::forward_list<Node>;
  Node *p_last_added = nullptr;
  vector<BucketT> data;
  KeyHasher k_hash;

private:

  size_t get_bucket_index(const KeyT &key) const {
    return k_hash(key) % M;
  }

public:
  HashLinkedMap()
    : data(vector<BucketT>(M))
  {
  }

  /**
   * Adds pair {key, value}, if element is not presented,
   * modify an existing value otherwise
   */
  void put(KeyT key, ValueT val) {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](Node &node) {
      return node.key == key;
    });
    if (it != end(bucket)) {
      // update exising value
      it->val = std::move(val);
    }
    else {
      // add new pair
      bucket.push_front({std::move(key), std::move(val), p_last_added, nullptr});
      auto &curr_node = *begin(bucket);
      if (p_last_added) {
        p_last_added->next = &curr_node;
      }
      p_last_added = &curr_node;
    }
  }

  /**
   * Deletes pair {key, *}, if element is presented
   */
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
        return true;
      }
      return false;
    });
  }

  /**
   * Return value of key, if such element exists
   */
  std::optional<std::string_view> get(KeyT key) const {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](const Node &node) {
      return node.key == key;
    });
    if (it != end(bucket)) {
      return {it->val};
    }
    else {
      return {};
    }
  }

  /**
   * Return value of the element inserted before key, if such element exists
   */
  std::optional<std::string_view> prev(KeyT key) const {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](const Node &node) {
      return node.key == key;
    });
    if (it != end(bucket) && it->prev) {
      return {it->prev->val};
    }
    else {
      return {};
    }
  }

  /**
   * Return value of the element inserted after key, if such element exists
   */
  std::optional<std::string_view> next(KeyT key) const {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](const Node &node) {
      return node.key == key;
    });
    if (it != end(bucket) && it->next) {
      return {it->next->val};
    }
    else {
      return {};
    }
  }
};

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  HashLinkedMap<std::string, std::string, StringHash> mp;

  auto print = [](std::optional<std::string_view> &opt) {
    if (opt.has_value())
      cout << opt.value() << "\n";
    else
      cout << "none\n";
  };

  string command, x, y;
  while (cin >> command) {
    if (command == "put") {
      cin >> x >> y;
      mp.put(move(x), move(y));
    }
    else if (command == "get") {
      cin >> x;
      auto opt = mp.get(move(x));
      print(opt);
    }
    else if (command == "prev") {
      cin >> x;
      auto opt = mp.prev(move(x));
      print(opt);
    }
    else if (command == "next") {
      cin >> x;
      auto opt = mp.next(move(x));
      print(opt);
    }
    else if (command == "delete") {
      cin >> x;
      mp.remove(move(x));
    }
    else {
      assert(0);
    }
  }
  return 0;
}
