#include <iostream>
#include <random>
#include <string>

int main(int argc, char** argv) {
    unsigned seed = (argc > 1) ? std::stoul(argv[1]) : 1;
    std::mt19937 rng(seed);

    int n = 500000;
    int q = 500000;
    std::cout << n << " " << q << "\n";

    for (int i = 0; i < n; ++i) {
        long long v = (long long)(rng() % 2000000000) - 1000000000;
        std::cout << v << (i + 1 == n ? "\n" : " ");
    }

    int curr_size = n;
    for (int qi = 0; qi < q; ++qi) {
        int op_kind;
        if (curr_size == 0)
            op_kind = 3;
        else
            op_kind = rng() % 6;
        char ops[] = {'A', 'R', 'O', 'I', 'E', 'M'};
        char op = ops[op_kind];
        std::cout << op << " ";
        if (op == 'A') {
            int l = 1 + rng() % curr_size;
            int r = l + rng() % (curr_size - l + 1);
            long long x = (long long)(rng() % 2000000000) - 1000000000;
            std::cout << l << " " << r << " " << x << "\n";
        } else if (op == 'R') {
            int l = 1 + rng() % curr_size;
            int r = l + rng() % (curr_size - l + 1);
            std::cout << l << " " << r << "\n";
        } else if (op == 'O') {
            int l = 1 + rng() % curr_size;
            int r = l + rng() % (curr_size - l + 1);
            int k = rng() % (r - l + 1);
            std::cout << l << " " << r << " " << k << "\n";
        } else if (op == 'I') {
            int k = rng() % (curr_size + 1);
            long long x = (long long)(rng() % 2000000000) - 1000000000;
            std::cout << k << " " << x << "\n";
            curr_size++;
        } else if (op == 'E') {
            int k = 1 + rng() % curr_size;
            std::cout << k << "\n";
            curr_size--;
        } else {
            int l = 1 + rng() % curr_size;
            int r = l + rng() % (curr_size - l + 1);
            std::cout << l << " " << r << "\n";
        }
    }
    return 0;
}
