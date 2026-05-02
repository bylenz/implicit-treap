#include <iostream>
#include <string>

#include "treap.hpp"

namespace {
// Estimación: cada respuesta numérica + '\n' ocupa ~12 chars en el peor caso
// (long long puede tener hasta 19 dígitos + signo). 16 es holgado y pre-reserva
// suficiente para evitar realocaciones del string de output.
constexpr int OUTPUT_RESERVE_PER_QUERY = 16;
}  // anonymous namespace

auto main() -> int {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int total_elements = 0;
    int total_queries = 0;
    std::cin >> total_elements >> total_queries;

    treap::init(((total_elements + total_queries) * 2) + 10);

    int root = 0;

    // Construcción inicial: insertar cada elemento al final del treap
    for (int i = 0; i < total_elements; ++i) {
        long long initial_value = 0;
        std::cin >> initial_value;
        int new_node = treap::create_node(initial_value);
        root = treap::merge(root, new_node);
    }

    std::string output;
    output.reserve(OUTPUT_RESERVE_PER_QUERY * total_queries);

    for (int queries = 0; queries < total_queries; ++queries) {
        char operation = '\0';
        std::cin >> operation;

        if (operation == 'A') {
            int left_index = 0;
            int right_index = 0;
            long long delta = 0;
            std::cin >> left_index >> right_index >> delta;
            root = treap::op_adding(root, left_index, right_index, delta);

        } else if (operation == 'R') {
            int left_index = 0;
            int right_index = 0;
            std::cin >> left_index >> right_index;
            root = treap::op_reversing(root, left_index, right_index);

        } else if (operation == 'O') {
            int left_index = 0;
            int right_index = 0;
            int rotation_count = 0;
            std::cin >> left_index >> right_index >> rotation_count;
            root = treap::op_right_rotations(root, left_index, right_index, rotation_count);

        } else if (operation == 'I') {
            int position_before = 0;
            long long value = 0;
            std::cin >> position_before >> value;
            root = treap::op_insert(root, position_before, value);

        } else if (operation == 'E') {
            int delete_position = 0;
            std::cin >> delete_position;
            root = treap::op_eliminate(root, delete_position);

        } else {  // operation == 'M'
            int left_index = 0;
            int right_index = 0;
            std::cin >> left_index >> right_index;
            auto [new_root, answer] = treap::op_minimum(root, left_index, right_index);
            root = new_root;
            output += std::to_string(answer);
            output += '\n';
        }
    }

    std::cout << output;
    return 0;
}
