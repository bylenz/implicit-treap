#pragma once
#include <random>
#include <vector>

struct Node {
    long long value;
    int size;
    long long min_val;
    long long lazy_add;
    bool lazy_rev;
    unsigned priority;
    int left, right;
};

namespace treap {

extern std::vector<Node> pool;
extern std::mt19937 rng;

void init(int reserve_hint);

auto create_node(long long value) -> int;

void apply_add(int node, long long delta);
void apply_rev(int node);
void push_down(int node);
void update(int node);

auto split(int root, int prefix_size) -> std::pair<int, int>;
auto merge(int left_root, int right_root) -> int;

auto op_adding(int root, int left_index, int right_index, long long delta) -> int;
auto op_reversing(int root, int left_index, int right_index) -> int;
auto op_right_rotations(int root, int left_index, int right_index, int rotation_count) -> int;
auto op_insert(int root, int position_before, long long value) -> int;
auto op_eliminate(int root, int delete_position) -> int;
auto op_minimum(int root, int left_index, int right_index) -> std::pair<int, long long>;

}  // namespace treap
