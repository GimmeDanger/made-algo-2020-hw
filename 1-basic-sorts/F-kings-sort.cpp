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

template <class Container>
void print(const Container &c, const char delim = ' ') {
  for (auto it = begin(c); it != end(c); it++)
    cout << *it << delim;
  if (!c.empty())
    cout << '\n';
}

class roman_number {
  string str_format{};
  int int_format{};

public:
  explicit roman_number() = default;
  explicit roman_number(string s)
      : str_format(move(s))
      , int_format(roman_to_int(str_format))
  {}

  [[nodiscard]] const string& to_str() const {
    return str_format;
  }

  [[nodiscard]] int to_int() const {
    return int_format;
  }

private:
  // Converts roman number @s into integer
  static int roman_to_int(const string &s) {
    int num = 0;
    for (int pos = 0; pos < s.size(); pos++) {
      int curr = roman_digit_to_int(s[pos]);
      if (pos < s.size() - 1) {
        auto opt = is_substract(s[pos], s[pos + 1]);
        if (opt.has_value()) {
          curr = opt.value();
          pos++;
        }
      }
      num += curr;
    }
    return num;
  }

  // Converts roman digit @ch into integer
  static int roman_digit_to_int(char ch) {
    switch (ch) {
    case 'I':
      return 1;
    case 'V':
      return 5;
    case 'X':
      return 10;
    case 'L':
      return 50;
    case 'C':
      return 100;
    case 'D':
      return 500;
    case 'M':
      return 1000;
    default:
      throw std::invalid_argument("");
    }
  }

  // helper method, checks if two roman digits should be treated as substraction
  static optional<int> is_substract(char lhs, char rhs) {
    if ((lhs == 'I' && (rhs == 'V' || rhs == 'X')) ||
        (lhs == 'X' && (rhs == 'L' || rhs == 'C')) ||
        (lhs == 'C' && (rhs == 'D' || rhs == 'M'))) {
      return {roman_digit_to_int(rhs) - roman_digit_to_int(lhs)};
    }
    return {};
  }
};

struct king_full_name {
  string name{};
  roman_number serial{};
  explicit king_full_name () = default;
  explicit king_full_name(string name_, string serial_)
      : name(move(name_))
      , serial(roman_number(move(serial_)))
  {}
  bool operator<(const king_full_name &that) const {
    if (name != that.name)
      return name < that.name;
    else
      return serial.to_int() < that.serial.to_int();
  }
};

ostream &operator<<(ostream &s, const king_full_name &obj) {
  s << obj.name << ' ' << obj.serial.to_str();
  return s;
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  // Get input
  int n;
  cin >> n;
  string input_name, input_serial;
  vector<king_full_name> kings;
  kings.reserve(n);
  for (int i = 0; i < n; i++) {
    cin >> input_name >> input_serial;
    kings.emplace_back(move(input_name), move(input_serial));
  }

  // Solve
  sort(begin(kings), end(kings));

  // Output result
  print(kings, '\n');
  return 0;
}
