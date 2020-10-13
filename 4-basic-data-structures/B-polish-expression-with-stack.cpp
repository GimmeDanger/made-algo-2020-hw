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
  [[nodiscard]] bool Empty() const {
    return size == 0;
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
      throw std::underflow_error("Error: trying to pop from empty vector.");
    }
    if (size < capacity / 4) {
      UpdateCapacity(max<size_t>(capacity / 2, 1));
    }
    size--;
  }
};

template<typename T>
class Stack {
  Vector<T> data;

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
    data.PopBack();
  }

  // O(1)
  [[nodiscard]] const T &GetTop() const {
    if (data.Empty()) {
      throw std::underflow_error(
          "Error: trying to get the top from an empty stack.");
    }
    return data[data.Size() - 1];
  }
};

int main() {
  FAST_IO();
  INPUT_SOURCE();

  Stack<int> st;
  for (char ch; cin >> ch;) {
    if (ch >= '0' && ch <= '9') {
      st.Push(ch - '0');
    } else {
      int b = st.GetTop();
      st.Pop();
      int a = st.GetTop();
      st.Pop();
      if (ch == '+') {
        st.Push(a + b);
      } else if (ch == '-') {
        st.Push(a - b);
      } else if (ch == '*') {
        st.Push(a * b);
      }
    }
  }
  cout << st.GetTop() << endl;
  return 0;
}
