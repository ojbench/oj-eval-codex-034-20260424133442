#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <vector>
#include <cstdlib>
#include <ctime>

// A generic skip list implementation that only relies on operator< for T.
// Equality is determined by: !(a < b) && !(b < a)
template<typename T>
class SkipList {
private:
    struct Node {
        T* key;                    // nullptr for head/sentinel
        std::vector<Node*> next;   // forward pointers

        explicit Node(int level, T* k = nullptr)
            : key(k), next(level, nullptr) {}

        ~Node() { delete key; }
    };

    Node* head;           // sentinel node without a key
    int level;            // current top level (1-based logical, indices 0..level-1)
    static constexpr int MAX_LEVEL = 32;

    static bool lessThan(const T& a, const T& b) {
        return a < b;
    }

    static bool equals(const T& a, const T& b) {
        return !lessThan(a, b) && !lessThan(b, a);
    }

    // Random level with probability p=0.5 per level increment
    int randomLevel() const {
        int lvl = 1;
        while (lvl < MAX_LEVEL && (std::rand() & 1)) ++lvl;
        return lvl;
    }

public:
    SkipList()
        : head(new Node(MAX_LEVEL, nullptr)), level(1) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    ~SkipList() {
        Node* cur = head->next[0];
        while (cur) {
            Node* nxt = cur->next[0];
            delete cur;
            cur = nxt;
        }
        delete head;
    }

    // Insert a value into the skip list. If the value already exists, do nothing.
    void insert(const T & item) {
        Node* update[MAX_LEVEL];
        Node* x = head;
        for (int i = level - 1; i >= 0; --i) {
            while (x->next[i] && lessThan(*(x->next[i]->key), item)) {
                x = x->next[i];
            }
            update[i] = x;
        }

        x = x->next[0];
        if (x && equals(*(x->key), item)) {
            return; // already exists
        }

        int newLevel = randomLevel();
        if (newLevel > level) {
            for (int i = level; i < newLevel; ++i) {
                update[i] = head;
            }
            level = newLevel;
        }

        Node* n = new Node(newLevel, new T(item));
        for (int i = 0; i < newLevel; ++i) {
            n->next[i] = update[i]->next[i];
            update[i]->next[i] = n;
        }
    }

    // Search for a value in the skip list
    bool search(const T & item) {
        Node* x = head;
        for (int i = level - 1; i >= 0; --i) {
            while (x->next[i] && lessThan(*(x->next[i]->key), item)) {
                x = x->next[i];
            }
        }
        x = x->next[0];
        return x && equals(*(x->key), item);
    }

    // Delete a value from the skip list. If the value does not exist in the skip list, do nothing.
    void deleteItem(const T & item) {
        Node* update[MAX_LEVEL];
        Node* x = head;
        for (int i = level - 1; i >= 0; --i) {
            while (x->next[i] && lessThan(*(x->next[i]->key), item)) {
                x = x->next[i];
            }
            update[i] = x;
        }
        x = x->next[0];
        if (!x || !equals(*(x->key), item)) {
            return; // not found
        }

        for (int i = 0; i < level; ++i) {
            if (update[i]->next[i] == x) {
                update[i]->next[i] = x->next[i];
            }
        }
        delete x;

        while (level > 1 && head->next[level - 1] == nullptr) {
            --level;
        }
    }
};

#endif
