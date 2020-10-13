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

template<typename T>
class LinkedList {
private:
  struct Node;
  using NodePtr = shared_ptr<Node>;
  struct Node {
    T value = T();
    NodePtr next;
    explicit Node(T value_) : value(std::move(value_)) {}
  };

  // Data
  NodePtr head;
  NodePtr tail;
  size_t size = 0;

public:
  // Constructor (rule of 5)
  explicit LinkedList() = default;
  LinkedList(const LinkedList<T> &other) = delete;
  LinkedList(LinkedList<T> &&other) = delete;
  LinkedList<T>& operator=(const LinkedList<T> &other) = delete;
  LinkedList<T>& operator=(LinkedList<T> &&other) = delete;

  // Methods
  [[nodiscard]] bool Empty() const {
    return size == 0;
  }

  [[nodiscard]] size_t Size() const {
    return size;
  }

  // O(1)
  void PushFront(const T& value) {
    NodePtr newNode = std::make_shared<Node>(value);
    if (Empty()) {
      head = tail = newNode;
    } else {
      newNode->next = head;
      head = newNode;
    }
    size++;
  }

  // O(1)
  void PushBack(const T& value) {
    NodePtr newNode = make_shared<Node>(value);
    if (Empty()) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    size++;
  }

  // O(N)
  void Erase(size_t index) {
    if (Empty())
      throw std::underflow_error("Error: erasing from an empty stack.");

    if (index >= size)
      throw std::out_of_range("Error: index is out of bound");

    if (index == 0) {
      PopFront();
      return;
    }

    NodePtr *prevPtr = &head;
    for (size_t i = 1; i < size; i++) {
      auto &prev = *prevPtr;
      auto &curr = (prev)->next;
      if (i == index) {
        curr = curr->next;
        prev->next = curr;
        if (index == --size)
          tail = prev;
        break;
      }
      prevPtr = &curr;
    }
  }

  // O(1)
  void PopFront() {
    if (Empty())
      throw std::underflow_error("Error: popping from an empty stack.");
    head = head->next;
    if (--size == 0)
      tail.reset();
  }

  // Getters
  [[nodiscard]] const NodePtr &GetHead() const {
    return head;
  }
  [[nodiscard]] const NodePtr &GetTail() const {
    return tail;
  }
};

template<typename T>
class Stack {
  LinkedList<T> ll;
public:

  // All constructors are default

  // Methods
  [[nodiscard]] bool Empty() const {
    return ll.Empty();
  }

  [[nodiscard]] size_t Size() const {
    return ll.Size();
  }

  // O(1)
  void Push(const T& value) {
    ll.PushFront(value);
  }

  // O(1)
  void Pop() {
    ll.PopFront();
  }

  // O(1)
  [[nodiscard]] const T &GetTop() const {
    return ll.GetHead()->value;
  }
};

template<typename T>
class MinStack {

  // Data
  Stack<std::pair<T, T>> st;

public:
  // All constructors are default

  // Methods
  [[nodiscard]] bool Empty() const {
    return st.Empty();
  }

  [[nodiscard]] size_t Size() const {
    return st.Size();
  }

  // O(1)
  void Push(const T& value) {
    auto prevMinValue = GetMin();
    st.Push({value, min(value, prevMinValue)});
  }

  // O(1)
  void Pop() {
    st.Pop();
  }

  // O(1)
  [[nodiscard]] const T &GetTop() const {
    if (Empty())
      throw std::underflow_error("Error: getting top from empty stack");
    return st.GetTop().first;
  }

  // O(1)
  [[nodiscard]] T GetMin() const {
    return st.Empty() ? std::numeric_limits<T>::max() : st.GetTop().second;
  }
};

int main() {
  FAST_IO();
  INPUT_SOURCE();

  MinStack<int> st;

  int n, t, x;
  cin >> n;
  while (n--) {
    cin >> t;
    if (t == 1 /*push back*/) {
      cin >> x;
      st.Push(x);
    }
    else if (t == 2 /*pop back*/) {
      st.Pop();
    }
    else if (t == 3 /*get min*/) {
      cout << st.GetMin() << "\n";
    }
  }

  return 0;
}
