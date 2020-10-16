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
class HashMap {
  static constexpr size_t M = 100'000;
  using BucketT = std::forward_list<std::pair<KeyT, ValueT>>;
  vector<BucketT> data;
  KeyHasher k_hash;

private:

  size_t get_bucket_index(const KeyT &key) const {
    return k_hash(key) % M;
  }

public:
  HashMap()
      : data(vector<BucketT>(M))
  {
  }

  /**
   * Adds pair {key, value}, if element is not presented,
   * modify an existing value otherwise
   */
  void put(KeyT key, ValueT val) {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](auto &node) {
      return node.first == key;
    });
    if (it != end(bucket)) {
      // update exising value
      it->second = std::move(val);
    }
    else {
      // add new pair
      bucket.push_front({std::move(key), std::move(val)});
    }
  }

  /**
   * Get value by key, if element is presented
   */
  std::optional<std::string_view> get(KeyT key) const {
    auto &bucket = data[get_bucket_index(key)];
    auto it = std::find_if(begin(bucket), end(bucket), [&key](auto &node) {
      return node.first == key;
    });
    if (it != end(bucket)) {
      return {it->second};
    }
    else {
      return {};
    }
  }

  /**
   * Deletes pair {key, *}, if element is presented
   */
  void remove(KeyT key) {
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

  HashMap<std::string, std::string, StringHash> mp;

  string command, x, y;
  while (cin >> command) {
    if (command == "put") {
      cin >> x >> y;
      mp.put(move(x), move(y));
    }
    else if (command == "get") {
      cin >> x;
      auto opt = mp.get(move(x));
      if (opt.has_value())
        cout << opt.value() << "\n";
      else
        cout << "none\n";
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
