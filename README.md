# C Data Structures Lite

Three clean, heavily commented **C11** data structures with demos and unit tests. Free and MIT licensed.

| Module | What it is |
|---|---|
| `vector` | Dynamic array with amortized O(1) growth |
| `slist` | Singly linked list (head + tail, reverse, insert/remove at index) |
| `stack` | Stack in two versions (array and linked) plus a bracket checker |

* Plain C11, no dependencies
* Zero warnings with `-Wall -Wextra -Wpedantic -std=c11`
* Every `malloc` is checked and every failure returns a clear status code
* 27 unit tests (345 checks) using a tiny framework you can read in five minutes

## Build and run

```bash
make          # builds the library, 3 demos and 3 test programs
make test     # runs every suite and prints a summary
./build/demos/demo_vector     # Windows: build\demos\demo_vector.exe
```

On Windows with MinGW-w64, use `mingw32-make` instead of `make`. Override the compiler with `make CC=clang`.

## Want the rest?

This is a free sample of the **C Data Structures & Algorithms Kit**, which has 12 modules: dynamic array, singly and doubly linked lists, stack, queue and deque, binary search tree, AVL tree, min-heap, hash table, graph algorithms (BFS, DFS, Dijkstra, topological sort), seven sorting algorithms and searching. It includes 114 tests, 12 demos and a 23-page PDF study guide.

**Full kit:** https://payhip.com/b/djq5l

**Also free:** [C Pointers in One Page](https://payhip.com/b/8eMNa), a printable A4 cheat sheet whose examples were all compiled and checked.

**C and C++ bundle** (data structures kit + C++ OOP projects + printable cheat sheets): https://payhip.com/b/aBFfK

## License

MIT. See `LICENSE`. The full kit is sold under a separate single-user license.

*Intended for learning and reference. Please follow your course's academic-integrity rules.*
