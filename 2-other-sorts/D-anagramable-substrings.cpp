#include <bits/stdc++.h>
#include "profile.h"
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

constexpr int alphabet_beg = 'a';
constexpr int alphabet_end = 'z' + 1;
constexpr int alphabet_size = alphabet_end - alphabet_beg;
using footprint_t = array<int, alphabet_size>;

// Computes footprint array @fp of string @s,
// where fp[i] == number of occurrences of character
// with alphabet position i in the given string,
// complexity: O(alphabet_size) = O(1)
footprint_t get_footprint(string_view s) {
  footprint_t fp{};
  fill(begin(fp), end(fp), 0);
  for (char ch : s)
    fp[ch - alphabet_beg]++;
  return fp;
}

// Checks if @s could be constructed with
// a footprint @fp of another string,
// complexity: O(alphabet_size) = O(1)
bool could_be_constructed(string_view s, const footprint_t &fpt) {
  auto fps = get_footprint(s);
  for (int i = 0; i < alphabet_size; i++)
    if (fps[i] > fpt[i])
      return false;
  return true;
}

// Computes the number of substrings of @s
// which could be constructed from letters of @t,
// complexity: O(n^2 + m), where n = len(s), m = len(t)
long long get_number_of_good_substrings_naive(const string &s, const string &t) {
  long long res = 0;
  const auto fpt = get_footprint(t);
  for (int i = 0; i < s.size(); i++)
    for (int j = i; j < s.size(); j++)
      res += could_be_constructed(string_view(s.data() + i, j - i + 1), fpt);
  return res;
}

// Computes the number of substrings of @s
// which could be constructed from letters of @t,
// complexity: O(n + m), where n = len(s), m = len(t)
long long get_number_of_good_substrings_fast(const string &s, const string &t) {

  // preprocess t for usage, compute number of digit occurrences
  const auto orig_fpt = get_footprint(t);
  auto fpt = orig_fpt;

  long long res = 0;
  auto process_last_segment =
      [&res](int &lo_prev, int &hi_prev, int lo, int hi) {
        long long len = hi - lo + 1;
        res += (len + 1) * len / 2;
        if (lo <= hi_prev && lo_prev <= hi_prev) {
          long long inter = hi_prev - lo + 1;
          res -= (inter + 1) * inter / 2;
        }
        lo_prev = lo;
        hi_prev = hi;
  };

  int lo_prev = -1, hi_prev = -1, lo = -1, hi = -1;
  for (lo = 0, hi = 0; lo <= hi && hi < s.size(); ) {
    size_t hi_pos = s[hi] - alphabet_beg;
    if (fpt[hi_pos] == 0) {
      // couldn't enlarge interval [lo, hi-1]
      // it should be processed and lo should be changed
      process_last_segment(lo_prev, hi_prev, lo, hi-1);
      // update curr segment to change lo
      while (fpt[hi_pos] == 0 && lo <= hi) {
        size_t lo_pos = s[lo] - alphabet_beg;
        if (orig_fpt[lo_pos])
          fpt[lo_pos]++;
        lo++;
      }
    }
    else {
      // interval [lo, hi-1] could be enlarged
      fpt[hi_pos]--;
      hi++;
    }
    hi = max(lo, hi);
  }
  process_last_segment(lo_prev, hi_prev, lo, hi-1);
  return res;
}

int main() {
  // Preps
  FAST_IO();
  INPUT_SOURCE();

  if (dbg) {
    random_device rd;
    mt19937 gen(rd());
    for (int len = 1; len < 20; len++) {
      for (int num = 1; num < alphabet_size; num++) {
        for (int tests = 100; tests >= 0; tests--) {
          uniform_int_distribution<> length_dis(0, len);
          uniform_int_distribution<> letters_dis(0, num);

          string s, t;
          for (int i = 0, len = length_dis(gen); i < len; i++)
            s.push_back(char(letters_dis(gen) + alphabet_beg));
          for (int i = 0, len = length_dis(gen); i < len; i++)
            t.push_back(char(letters_dis(gen) + alphabet_beg));

          long long res_naive = get_number_of_good_substrings_naive(s, t);
          long long res_fast = get_number_of_good_substrings_fast(s, t);
          if (res_naive != res_fast) {
            cout << res_naive << endl << res_fast << endl;
            cout << s << endl << t << endl;
            assert(0);
          }
        }
      }
    }
    printf("All tests passsed!\n");
  }

  int n, m;
  cin >> n >> m;
  string s, t;
  cin >> s >> t;
  cout << get_number_of_good_substrings_fast(s, t) << endl;

  return 0;
}
