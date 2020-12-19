#include <bits/stdc++.h>

using namespace std;

#define dbg 0

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

class Trie {

    struct Node;
    using NodePtr = unique_ptr<Node>;
    static constexpr char ALPHABET_START = 'a';
    static constexpr size_t RADIX = 26;
    static constexpr int INVALID_ID = -1;

    static size_t get_index(char ch) {
        return static_cast<size_t>(ch - ALPHABET_START);
    }

    struct Node {
        int s_id = INVALID_ID;
        array<NodePtr, RADIX> next;
    };

    NodePtr root;

    void insert_impl(NodePtr &node, string_view s, const int s_id) {
        if (node == nullptr) {
            node = make_unique<Node>();
        }
        if (s.empty()) {
            node->s_id = s_id;
            return;
        }
        insert_impl(node->next[get_index(s.front())], s.substr(1), s_id);
    }

    void collect_impl(const NodePtr &node, string_view s, unordered_set<int>& st) const {
        if (node != nullptr && node->s_id != INVALID_ID) {
            st.insert(node->s_id);
        }
        if (node == nullptr || s.empty()) {
            return;
        }
        collect_impl(node->next[get_index(s.front())], s.substr(1), st);
    }

public:
    explicit Trie() = default;

    void insert(const string_view s, const int s_id) {
        insert_impl(root, s, s_id);
    }

    void collect(const string_view prefix, unordered_set<int>& st) const {
        collect_impl(root, prefix, st);
    }
};

void solve(istream &is, ostream &os) {
    static constexpr int MAX_STR_LEN = 30;
    int dict_size = 0;
    string t, s;
    is >> t >> dict_size;

    Trie dict;
    unordered_map<string, int> s_to_id;
    for (int id = 0; id < dict_size; id++) {
        is >> s;
        dict.insert(s, id);
        s_to_id.emplace(s, id);
    }

    unordered_set<int> found;
    for (int pos = 0; pos < t.size(); pos++) {
        size_t substr_size = min<size_t>(t.size() - pos, MAX_STR_LEN);
        dict.collect(string_view(t.data() + pos, substr_size), found);
    }
    for (int id = 0; id < dict_size; id++) {
        os << (found.count(id) ? "Yes" : "No") << '\n';
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
