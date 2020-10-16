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

class IntegerHash {
  static constexpr size_t A = 14753;
  static constexpr size_t P = 253812577607;
public:
  size_t operator()(const int key) const {
    return (A * static_cast<size_t>(key)) % P;
  }
};

template <typename KeyT, typename KeyHasher>
class HashSet {

  static constexpr size_t M = 2'000'000;

  struct Node {
    static constexpr int INVALID = std::numeric_limits<int>::min();
    enum class Status {
      EMPTY,
      OCCUPIED,
      TOMBSTONE
    };
    KeyT key = INVALID;
    Status status = Status::EMPTY;
  };

  size_t size = 0;
  size_t rip_count = 0;

  std::vector<Node> data;
  KeyHasher k_hash;

private:

  [[nodiscard]] bool isEmptyPos(size_t i) const {
    return data[i].status == Node::Status::EMPTY;
  }

  [[nodiscard]] bool isOccupiedPos(size_t i) const {
    return data[i].status == Node::Status::OCCUPIED;
  }

  [[nodiscard]] bool isTombstonePos(size_t i) const {
    return data[i].status == Node::Status::TOMBSTONE;
  }

public:
  HashSet()
      : data(std::vector<Node>(M))
  {
  }

  /**
   * Adds key, if element is not presented,
   */
  void insert(KeyT key) {
    if (size == M) {
      throw std::overflow_error("Error: full table.");
    }
    const size_t h = k_hash(key);
    for (size_t i = 0; i < M; i++) {
      size_t j = (h + i) % M;
      if (isOccupiedPos(j) && data[j].key == key) {
        return;
      }
      else if (isEmptyPos(j) || isTombstonePos(j)) {
        data[j] = {key, Node::Status::OCCUPIED};
        size++;
        return;
      }
    }
  }

  /**
   * Get value by key, if element is presented
   */
  bool exists(KeyT key) const {
    const size_t h = k_hash(key);
    for (size_t i = 0; i < M; i++) {
      size_t j = (h + i) % M;
      if (isOccupiedPos(j) && data[j].key == key) {
        return true;
      }
      else if (isEmptyPos(j)) {
        return false;
      }
    }
    return false;
  }

  /**
   * Delete key, if element is presented
   */
  void remove(KeyT key) {
    const size_t h = k_hash(key);
    for (size_t i = 0; i < M; i++) {
      size_t j = (h + i) % M;
      if (isOccupiedPos(j) && data[j].key == key) {
        data[j] = {Node::INVALID, Node::Status::TOMBSTONE};
        rip_count++;
        size--;
        // if (rip_count + size > M / 2)
        //   rehashing();
        return;
      }
      else if (isEmptyPos(j)) {
        return;
      }
    }
  }
};

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  HashSet<int, IntegerHash> mp;

  int x;
  string command;
  while (cin >> command) {
    if (command == "insert") {
      cin >> x;
      mp.insert(x);
    }
    else if (command == "exists") {
      cin >> x;
      cout << (mp.exists(x) ? "true" : "false") << "\n";
    }
    else if (command == "delete") {
      cin >> x;
      mp.remove(x);
    }
    else {
      assert(0);
    }
  }
  return 0;
}
