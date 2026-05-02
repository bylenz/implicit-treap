# Implicit Treap — EDA, Project 3

Implicit BST based on a randomized treap with lazy propagation. Supports
range operations (add, reverse, rotate, minimum) and insertion/deletion by
position. All operations run in **O(log n) expected**.

---

## Key concepts

### What is a Treap?

A treap is a binary search tree (BST) where each node is assigned a **random
priority** and the tree maintains the heap property with respect to it. The
randomness guarantees the tree stays balanced with high probability, giving
O(log n) on all operations.

### What does "implicit" mean?

In a regular BST the key of each node is its stored value. In an implicit treap
the **key is the position in the sequence** — and it is never stored explicitly.
It is derived on the fly by counting nodes in the left subtree (`size`). This
lets us use the tree as a **dynamic array** where any subrange can be isolated,
transformed, and reassembled in O(log n).

### Primitive operations: split and merge

Everything else is built on top of two operations:

- **`split(root, k)`** — splits the tree into two: the first `k` elements and
  the rest. O(log n).
- **`merge(left, right)`** — joins two treaps while maintaining the heap
  property. O(log n).

Every range operation `[l, r]` follows the same pattern:

```
split at l-1  →  split at (r-l+1)  →  operate on the middle segment  →  merge
```

### Lazy propagation

Instead of visiting every node in the range, the pending operation is stored at
the root of the segment and pushed down (`push_down`) only when a descent is
needed. Two lazy fields coexist in each node:

| Field       | Purpose                                              |
|-------------|------------------------------------------------------|
| `lazy_add`  | Accumulated sum pending propagation to children      |
| `lazy_rev`  | Whether the subtree is pending a reversal            |

### Node pool (arena allocator)

Nodes are not heap-allocated with `new`. They all live in a `std::vector<Node>`
called `pool`. "Pointers" are plain **integer indices** (`int`), not real
pointers. Benefits: cache-friendly, no fragmentation, no `malloc` overhead.

---

## Supported operations

| Code | Signature                    | Description                                       |
|------|------------------------------|---------------------------------------------------|
| `A`  | `A l r delta`                | Add `delta` to every element in `[l, r]`          |
| `R`  | `R l r`                      | Reverse the subarray `[l, r]`                     |
| `O`  | `O l r k`                    | Right-rotate the range `[l, r]` by `k` positions  |
| `I`  | `I pos val`                  | Insert `val` at position `pos` (1-indexed)        |
| `E`  | `E pos`                      | Delete the element at position `pos`              |
| `M`  | `M l r`  → prints minimum    | Minimum of the subarray `[l, r]`                  |

---

## Build

### 1. Configure

```bash
cmake -B build                                    # release (O2, NDEBUG)
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug     # debug (ASan + UBSan)
```

### 2. Grant execute permissions to the scripts (first time only)

```bash
chmod +x tests/stress.sh tests/bench.sh
```

### 3. Compile and run

All targets are invoked with `make -C <directory>`:

| Command                      | What it does                                                |
|------------------------------|-------------------------------------------------------------|
| `make -C build`              | Compiles `build/main` in release mode                       |
| `make -C build-debug`        | Compiles with AddressSanitizer and UndefinedBehaviorSanitizer |
| `make -C build run`          | Compiles and runs with `tests/sample.in`                    |
| `make -C build test_sample`  | Compiles, runs, and diffs output against `tests/sample.out` |
| `make -C build stress`       | Stress test: 200 random cases against the brute-force       |
| `make -C build bench`        | Benchmark with n=q=500 000                                  |

To change the number of stress-test iterations:

```bash
./tests/stress.sh 500
```

---

## Input format

```
N Q
a1 a2 ... aN
<operation 1>
<operation 2>
...
```

- `N`: initial number of elements
- `Q`: number of operations
- Indices are **1-based**

---

## Project structure

```
.
├── include/
│   └── treap.hpp        public interface (Node, namespace treap)
├── src/
│   ├── treap.cpp        split, merge, lazy propagation, range operations
│   └── main.cpp         I/O, parsing, operation dispatcher
├── tests/
│   ├── brute.cpp        O(n)-per-op brute-force for correctness validation
│   ├── gen.cpp          random small-case generator
│   ├── gen_big.cpp      large-case generator (benchmark)
│   ├── stress.sh        runs main vs brute on N random seeds
│   ├── bench.sh         times main on a 500k-operation input
│   └── sample.in / .out example case from the problem statement
├── report/
│   └── report.tex       implementation report
└── CMakeLists.txt
```
