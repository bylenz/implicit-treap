#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) std::cin >> a[i];

    std::string output;

    for (int qi = 0; qi < q; ++qi) {
        char op;
        std::cin >> op;
        if (op == 'A') {
            int l, r;
            long long x;
            std::cin >> l >> r >> x;
            for (int i = l - 1; i <= r - 1; ++i) a[i] += x;
        } else if (op == 'R') {
            int l, r;
            std::cin >> l >> r;
            std::reverse(a.begin() + (l - 1), a.begin() + r);
        } else if (op == 'O') {
            int l, r, k;
            std::cin >> l >> r >> k;
            int len = r - l + 1;
            k %= len;
            if (k > 0) {
                std::rotate(a.begin() + (l - 1), a.begin() + (r - k), a.begin() + r);
            }
        } else if (op == 'I') {
            int k;
            long long x;
            std::cin >> k >> x;
            a.insert(a.begin() + k, x);
        } else if (op == 'E') {
            int k;
            std::cin >> k;
            a.erase(a.begin() + (k - 1));
        } else {  // M
            int l, r;
            std::cin >> l >> r;
            long long mn = a[l - 1];
            for (int i = l; i <= r - 1; ++i) mn = std::min(mn, a[i]);
            output += std::to_string(mn);
            output += '\n';
        }
    }

    std::cout << output;
    return 0;
}
