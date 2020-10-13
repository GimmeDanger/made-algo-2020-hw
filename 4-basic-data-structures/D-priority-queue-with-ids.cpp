#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <memory>
using namespace std;

#define dbg 0

#if dbg
#include <fstream>
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

template <typename T>
class Vector {
private:
  // Data
  std::unique_ptr<T[]> data;
  size_t size = 0;
  size_t capacity = 0;

  void UpdateCapacity(size_t newCapacity) {
    if (size > newCapacity) {
      throw std::invalid_argument(
          "Error: new capacity must be greater than size.");
    }
    auto newData = std::make_unique<T[]>(newCapacity);
    auto *dataBegin = data.get();
    auto *dataEnd = data.get() + min(capacity, newCapacity);
    std::move(dataBegin, dataEnd, newData.get());
    data = std::move(newData);
    capacity = newCapacity;
  }

public:
  // Construction operators
  explicit Vector() = default;
  explicit Vector(size_t size)
      : data(size ? std::make_unique<T[]>(size) : nullptr)
      , size(size)
      , capacity(size)
  {
  }
  ~Vector() = default;
  Vector(const Vector &other) = delete;
  Vector &operator=(const Vector &other) = delete;
  Vector(Vector &&other) noexcept = delete;
  Vector &operator=(Vector &&other) noexcept = delete;

  // Getters
  [[nodiscard]] size_t Size() const {
    return size;
  }

  [[nodiscard]] size_t Capacity() const {
    return capacity;
  }

  [[nodiscard]] bool Empty() const {
    return size == 0;
  }

  [[nodiscard]] T Back() const {
    if (Empty()) {
      throw std::underflow_error("Error: empty vector.");
    }
    return data[size - 1];
  }

  T& operator[](size_t index) {
    if (index >= size) {
      throw std::invalid_argument("Error: index is out of bounds.");
    }
    return data[index];
  }
  const T& operator[](size_t index) const {
    if (index >= size) {
      throw std::invalid_argument("Error: index is out of bounds.");
    }
    return data[index];
  }

  // Insert element at back, O(1) amortized
  void PushBack(const T &value) {
    if (size >= capacity) {
      UpdateCapacity(max<size_t>(2 * capacity, 1));
    }
    data[size++] = value;
  }

  // Erase element at back, O(1) amortized
  void PopBack() {
    if (Empty()) {
      throw std::underflow_error("Error: empty vector.");
    }
    if (size < capacity / 4) {
      UpdateCapacity(max<size_t>(capacity / 2, 1));
    }
    size--;
  }
};

template<typename KeyT>
class MinPriorityQueue {
  // Heap internal storage
  using MarkT = size_t;
  using Node = std::pair<KeyT, MarkT>;
  Vector<Node> heap;

  // Mapping from unique marks to node indexes in heap
  std::unordered_map<MarkT, size_t> mapping;

public:
  // All constructors are default

  // O(1)
  [[nodiscard]] bool Empty() const {
    return heap.Empty();
  }

  // O(1)
  [[nodiscard]] size_t Size() const { return heap.Size(); }

  /**
   * Insert new node into the queue
   * Complexity: O(logN)
   */
  void Push(const KeyT &key, const MarkT &mark) {
    heap.PushBack({std::numeric_limits<KeyT>::max(), mark});
    mapping[mark] = LastId();
    DecreaseKeyInternal(LastId(), key);
  }

  /**
   * Extract node with minimal key from the queue
   * Complexity: O(logN)
   */
  [[nodiscard]] Node ExtractMin() {
    auto ret = GetMin();
    SwapNodes(MinId(), LastId());

    MarkT mark = Mark(LastId());
    mapping.erase(mark);
    heap.PopBack();

    Heapify(MinId());
    return ret;
  }

  /**
   * Decrease key of the node with @mark to new value @newKey
   * Complexity: O(log(size))
  */
  void DecreaseKey(const KeyT &newKey, const MarkT &mark) {
    CheckUnderflow();
    auto it = mapping.find(mark);
    if (it == mapping.end())
      return;
    size_t nodeIndex = it->second;
    DecreaseKeyInternal(nodeIndex, newKey);
  }

  // O(1)
  [[nodiscard]] Node GetMin() const {
    CheckUnderflow();
    return heap[MinId()];
  }

private:

  // Internal methods

  void CheckUnderflow() const {
    if (Empty()) {
      throw std::underflow_error("Error: priority queue is empty.");
    }
  }

  [[nodiscard]] size_t MinId() const {
    return 0;
  }

  [[nodiscard]] size_t LastId() const {
    return Size() - 1;
  }

  static size_t Parent(size_t i) {
    return (i + 1) / 2 - 1;
  }

  static size_t Left(size_t i) {
    return 2 * (i + 1) - 1;
  }

  static size_t Right(size_t i) {
    return 2 * (i + 1);
  }

  void SwapNodes(size_t i, size_t j) {
    std::swap(heap[i], heap[j]);
    std::swap(mapping[Mark(i)], mapping[Mark(j)]);
  }

  [[nodiscard]] KeyT &Key(size_t i) {
    return heap[i].first;
  }

  [[nodiscard]] MarkT &Mark(size_t i) {
    return heap[i].second;
  }

  /**
   * Restore heap structure
   * if i-th node violates head property
   * so that val_i > max(val_l, val_r)
   *
   *        (val_i)
   *       /       \
   *   (val_l)   (val_r)
   *
   * Complexity: O(log(size))
  */
  void Heapify(size_t i) {
    // Underflow is checked in public methods
    size_t l = Left(i), r = Right(i), smallest = i;
    if (l < Size() && Key(l) < Key(smallest))
      smallest = l;
    if (r < Size() && Key(r) < Key(smallest))
      smallest = r;
    if (i != smallest) {
      SwapNodes(i, smallest);
      Heapify(smallest);
    }
  }

  /**
   * Decrease key of selected node,
   * subroutine of push method
   * Complexity: O(log(size))
  */
  void DecreaseKeyInternal(size_t i, KeyT newKey) {
    CheckUnderflow();
    if (newKey > Key(i)) {
      throw std::invalid_argument("Error: new key value is greater than curr.");
    }
    Key(i) = newKey;
    while (i != MinId() && Key(i) < Key(Parent(i))) {
      SwapNodes(i, Parent(i));
      i = Parent(i);
    }
  }
};

int main() {
  FAST_IO();
  INPUT_SOURCE();

  size_t command_cnt = 0;
  MinPriorityQueue<int> pq;

  for (string command; cin >> command;) {
    command_cnt++;
    if (command == "push") {
      int key;
      cin >> key;
      pq.Push(key, command_cnt);
    } else if (command == "decrease-key") {
      int mark, key;
      cin >> mark >> key;
      pq.DecreaseKey(key, mark);
    } else if (command == "extract-min") {
      if (pq.Empty()) {
        cout << "*\n";
      } else {
        auto minNode = pq.ExtractMin();
        cout << minNode.first << " " << minNode.second << "\n";
      }
    }
  }

  return 0;
}
