#include <algorithm>
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
class CyclicDeque {
private:
  // Data
  std::unique_ptr<T[]> data;
  size_t capacity = 0;
  size_t size = 0; //< len of [head, tail)
  size_t head = 0; //< index of the first element
  size_t tail = 0; //< index after the last element

  void UpdateCapacity(size_t newCapacity) {
    if (size > newCapacity) {
      throw std::invalid_argument(
          "Error: new capacity must be greater than size.");
    }
    auto newData = std::make_unique<T[]>(newCapacity);
    if (head < tail || capacity == 0) {
      // Case 1: [ ... head ***** tail ... ]
      auto *dataBegin = data.get() + head;
      auto *dataEnd = data.get() + tail;
      std::move(dataBegin, dataEnd, newData.get());
    }
    else {
      // Case 2: [ *** tail ... head *** ]
      // Copy [head, capacity) part
      auto *dataBegin = data.get() + head;
      auto *dataEnd = data.get() + capacity;
      std::move(dataBegin, dataEnd, newData.get());
      // Copy [0, tail) part
      dataBegin = data.get();
      dataEnd = data.get() + tail;
      std::move(dataBegin, dataEnd, newData.get() + (capacity - head));
    }
    head = 0;
    tail = size;
    capacity = newCapacity;
    data = std::move(newData);
  }

  [[nodiscard]] size_t GetNext(size_t pos) const {
    return (pos != capacity - 1) ? pos + 1 : 0;
  }

  [[nodiscard]] size_t GetPrev(size_t pos) const {
    return (pos != 0) ? pos - 1 : capacity - 1;
  }

public:
  // Construction operators
  explicit CyclicDeque() = default;
  explicit CyclicDeque(size_t size)
      : data(size ? std::make_unique<T[]>(size) : nullptr)
      , capacity(size)
      , size(size)
      , head(0)
      , tail(size)
  {
  }
  ~CyclicDeque() = default;
  CyclicDeque(const CyclicDeque &other) = delete;
  CyclicDeque &operator=(const CyclicDeque &other) = delete;
  CyclicDeque(CyclicDeque &&other) noexcept = delete;
  CyclicDeque &operator=(CyclicDeque &&other) noexcept = delete;

  // Getters
  [[nodiscard]] size_t Size() const {
    return size;
  }
  [[nodiscard]] bool Empty() const {
    return size == 0;
  }
  T& operator[](size_t index) {
    if (index >= size) {
      throw std::invalid_argument("Error: index is out of bounds.");
    }
    return data[(head + index) % capacity];
  }
  const T& operator[](size_t index) const {
    if (index >= size) {
      throw std::invalid_argument("Error: index is out of bounds.");
    }
    return data[(head + index) % capacity];
  }

  // Insert element at back, O(1) amortized
  void PushBack(const T &value) {
    if (size >= capacity) {
      UpdateCapacity(max<size_t>(2 * capacity, 1));
    }
    data[tail] = value;
    tail = GetNext(tail);
    size++;
  }

  // Insert element at front, O(1) amortized
  void PushFront(const T &value) {
    if (size >= capacity) {
      UpdateCapacity(max<size_t>(2 * capacity, 1));
    }
    data[head] = value;
    head = GetPrev(head);
    size++;
  }

  // Erase element at back, O(1) amortized
  void PopBack() {
    if (Empty()) {
      throw std::underflow_error("Error: trying to pop from empty deque.");
    }
    if (size < capacity / 4) {
      UpdateCapacity(max<size_t>(capacity / 2, 1));
    }
    tail = GetPrev(tail);
    size--;
  }

  // Erase element at front, O(1) amortized
  void PopFront() {
    if (Empty()) {
      throw std::underflow_error("Error: trying to pop from empty deque.");
    }
    if (size < capacity / 4) {
      UpdateCapacity(max<size_t>(capacity / 2, 1));
    }
    head = GetNext(head);
    size--;
  }

  // O(1)
  [[nodiscard]] const T &GetFront() const {
    if (Empty()) {
      throw std::underflow_error("Error: trying to get the front of an empty deque.");
    }
    return data[head];
  }

  [[nodiscard]] const T &GetBack() const {
    if (Empty()) {
      throw std::underflow_error("Error: trying to get the front of an empty deque.");
    }
    return data[GetPrev(tail)];
  }
};

template<typename T>
class Queue {
  CyclicDeque<T> data;

public:
  // All constructors are default

  // O(1)
  [[nodiscard]] bool Empty() const {
    return data.Empty();
  }

  // O(1)
  [[nodiscard]] size_t Size() const {
    return data.Size();
  }
  
  // O(1) amortized
  void Push(const T &value) {
    data.PushBack(value);
  }

  // O(1) amortized
  void Pop() {
    data.PopFront();
  }

  // O(1)
  [[nodiscard]] const T &GetFront() const {
    return data.GetFront();
  }
};

int main() {
  FAST_IO();
  INPUT_SOURCE();

  Queue<int> q;

  char ch;
  int m, n;
  cin >> m;
  while (m--) {
    cin >> ch;
    if (ch == '+') {
      cin >> n;
      q.Push(n);
    }
    else if (ch == '-') {
      cout << q.GetFront() << "\n";
      q.Pop();
    }
  }
  return 0;
}
