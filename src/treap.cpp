#include "treap.hpp"

#include <chrono>

namespace treap {

std::vector<Node> pool;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

void init(int reserve_hint) {
    pool.clear();
    pool.reserve(reserve_hint);
    pool.push_back(Node{});  // pool[0] = nodo nulo (size=0, lazies neutros)
}

auto create_node(long long value) -> int {
    Node node;
    node.value = value;
    node.size = 1;
    node.min_val = value;
    node.lazy_add = 0;
    node.lazy_rev = false;
    node.priority = rng();
    node.left = 0;
    node.right = 0;
    pool.push_back(node);
    return static_cast<int>(pool.size()) - 1;
}

void apply_add(int node, long long delta) {  // O(1)
    if (node == 0) {
        return;
    }
    pool[node].value += delta;
    pool[node].min_val += delta;
    pool[node].lazy_add += delta;
}

void apply_rev(int node) {  // O(1)
    if (node == 0) {
        return;
    }
    std::swap(pool[node].left, pool[node].right);
    pool[node].lazy_rev = !pool[node].lazy_rev;
}

void push_down(int node) {  // O(1)
    if (pool[node].lazy_add != 0) {
        apply_add(pool[node].left, pool[node].lazy_add);
        apply_add(pool[node].right, pool[node].lazy_add);
        pool[node].lazy_add = 0;
    }

    if (pool[node].lazy_rev) {
        apply_rev(pool[node].left);
        apply_rev(pool[node].right);
        pool[node].lazy_rev = false;
    }
}

void update(int node) {  // O(1)
    if (node == 0) {
        return;
    }

    int left = pool[node].left;
    int right = pool[node].right;

    pool[node].size = 1 + pool[left].size + pool[right].size;
    pool[node].min_val = pool[node].value;
    if (left != 0) {
        pool[node].min_val = std::min(pool[node].min_val, pool[left].min_val);
    }
    if (right != 0) {
        pool[node].min_val = std::min(pool[node].min_val, pool[right].min_val);
    }
}

auto split(int root, int prefix_size) -> std::pair<int, int> {
    if (root == 0) {
        return {0, 0};
    }
    push_down(root);

    int left = pool[root].left;
    int right = pool[root].right;
    int left_size = pool[left].size;

    if (prefix_size <= left_size) {
        auto [LL, new_left] = split(left, prefix_size);
        pool[root].left = new_left;
        update(root);
        return {LL, root};
    }
    auto [new_right, RR] = split(right, prefix_size - left_size - 1);
    pool[root].right = new_right;
    update(root);
    return {root, RR};
}

auto merge(int left_root, int right_root) -> int {
    if (left_root == 0) {
        return right_root;
    }
    if (right_root == 0) {
        return left_root;
    }

    if (pool[left_root].priority > pool[right_root].priority) {
        push_down(left_root);
        pool[left_root].right = merge(pool[left_root].right, right_root);
        update(left_root);
        return left_root;
    }
    push_down(right_root);
    pool[right_root].left = merge(left_root, pool[right_root].left);
    update(right_root);
    return right_root;
}

auto op_adding(int root, int left_index, int right_index, long long delta) -> int {
    auto [T1, T2] = split(root, left_index - 1);
    auto [M, T3] = split(T2, right_index - left_index + 1);

    apply_add(M, delta);

    return merge(T1, merge(M, T3));
}

auto op_reversing(int root, int left_index, int right_index) -> int {
    auto [T1, T2] = split(root, left_index - 1);
    auto [M, T3] = split(T2, right_index - left_index + 1);

    apply_rev(M);

    return merge(T1, merge(M, T3));
}

auto op_right_rotations(int root, int left_index, int right_index, int rotation_count) -> int {
    int range_length = right_index - left_index + 1;
    rotation_count %= range_length;
    if (rotation_count == 0) {
        return root;
    }
    auto [T1, T2] = split(root, left_index - 1);
    auto [M, T3] = split(T2, right_index - left_index + 1);

    auto [head, tail] = split(M, range_length - rotation_count);

    M = merge(tail, head);

    return merge(T1, merge(M, T3));
}

auto op_insert(int root, int position_before, long long value) -> int {
    int new_node = create_node(value);
    auto [T1, T2] = split(root, position_before);

    return merge(merge(T1, new_node), T2);
}

auto op_eliminate(int root, int delete_position) -> int {
    auto [T1, T2] = split(root, delete_position - 1);
    auto [M, T3] = split(T2, 1);

    return merge(T1, T3);
}

auto op_minimum(int root, int left_index, int right_index) -> std::pair<int, long long> {
    auto [T1, T2] = split(root, left_index - 1);
    auto [M, T3] = split(T2, right_index - left_index + 1);

    long long minimun = pool[M].min_val;

    return {merge(T1, merge(M, T3)), minimun};
}

}  // namespace treap
