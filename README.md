# Project 2 — Memory Allocation

**Course:** COSC1114 Operating Systems Principles, RMIT
**Team:** Project 2 Group 4
**Members:** S4204551 · S4197581 · S4000196 · S4055262

---

## Task Completion Status

| Task | Subtask | Status |
|---|---|---|
| Task 1 — Solution Design | Allocated list + free list (doubly linked lists), global accounting kept `static` inside each allocator file | ✅ Completed |
| Task 2 — Memory Allocation | `alloc()` searches the free list first, otherwise grows the heap with `sbrk()`; fixed partitions of 32, 64, 128, 256, 512 bytes | ✅ Completed |
| Task 3 — Allocation Strategies | First fit | ✅ Completed |
| | Best fit | ✅ Completed |
| | Quick fit (5 separate free lists, one per partition size) | ✅ Completed |
| Task 4 — Memory Deallocation | `dealloc()` finds the chunk in the allocated list and moves it to the end of the free list; terminates with an error if the chunk was never allocated | ✅ Completed |

All three programs compile with `-Wall -Werror` and run cleanly under Valgrind (no leaks, 0 errors) on the RMIT Core Teaching Servers.

---

## How to Build and Run on the Core Teaching Servers

Works on `titan.csit.rmit.edu.au`, `jupiter.csit.rmit.edu.au` or `saturn.csit.rmit.edu.au`.

### 1. Log in and unzip

```bash
ssh <student_id>@titan.csit.rmit.edu.au
unzip project2_group4.zip
cd project2_group4
```

### 2. Build

```bash
make all      # or just: make
```

This builds three programs into `bin/`: `firstfit`, `bestfit`, `quickfit`.

### 3. Create a test datafile

```bash
bash p2_gen.sh 20 > datafile
```

### 4. Run

```bash
./bin/firstfit datafile
./bin/bestfit datafile
./bin/quickfit datafile
```

Each program reads the `alloc: N` / `dealloc` lines in the datafile (dealloc frees the most recent allocation, LIFO), then prints once at the end:

- the allocated list — address, total size and used size of each chunk
- the free list — address and total size of each chunk (quick fit prints one free list per partition size)

### 5. Clean up

```bash
make clean    # removes bin/ and obj/
```

---

## Memory Testing

```bash
valgrind --leak-check=full --show-leak-kinds=all ./bin/<program> datafile
```

---

## File Structure

| Path | Purpose |
|---|---|
| `firstfit/` | First fit `alloc()` / `dealloc()` and its `main()` |
| `bestfit/` | Best fit `alloc()` / `dealloc()` and its `main()` |
| `quickfit/` | Quick fit `alloc()` / `dealloc()` and its `main()` |
| `utils/alloc_list.*` | Doubly linked list used for the allocated and free lists |
| `utils/mem_common.*` | Partition sizes and `sbrk()` helper shared by all strategies |
| `utils/driver.*` | Reads the datafile and calls `alloc()` / `dealloc()` |
| `Makefile` | `make all` builds all three programs, `make clean` removes build files |
| `p2_gen.sh` | Provided datafile generator |
