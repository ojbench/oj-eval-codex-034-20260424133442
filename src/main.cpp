#include <bits/stdc++.h>
#include SkipList.hpp

// This main is a simple harness to exercise the SkipList with basic I/O:
// Commands:
// 1 x : insert x
// 2 x : delete x
// 3 x : search x (prints 1 if found else 0)
int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    SkipList<long long> sl;
    int op; long long x;
    while (std::cin >> op >> x) {
        if (op == 1) sl.insert(x);
        else if (op == 2) sl.deleteItem(x);
        else if (op == 3) std::cout << (sl.search(x) ? 1 : 0) << '\n';
    }
    return 0;
}
