# Task 1 Design — Dynamic Memory Allocator

## Overview

We implement `alloc()` and `dealloc()` using two linked lists:
- **Allocated list** — tracks every chunk currently in use
- **Free list** — tracks every chunk that has been freed and can be reused

Memory chunks are always one of five fixed partition sizes: **32, 64, 128, 256, or 512 bytes**.

---

## Data Structures

### `allocation_t` — the memory chunk

Stores information about a single chunk of memory.

```c
typedef struct {
    size_t total_size;   // actual partition size given (32/64/128/256/512)
    size_t used_size;    // size the caller requested
    void  *space;        // pointer to the start of the memory chunk
} allocation_t;
```

- `total_size` is always one of the five fixed sizes (rounded up from the request)
- `used_size` is the original request — the difference is internal fragmentation
- `space` is the address returned to the caller

### `alloc_node_t` — linked list node

Wraps an `allocation_t` so it can be stored in a doubly-linked list.

```c
typedef struct alloc_node_t {
    struct alloc_node_t *next;   // next node in the list
    struct alloc_node_t *prev;   // previous node in the list
    allocation_t        *alloc;  // the chunk this node represents
} alloc_node_t;
```

A doubly-linked list is used so nodes can be removed from any position in O(1).

### `alloc_list_t` — the list

```c
typedef struct {
    alloc_node_t *head;   // first node
    alloc_node_t *tail;   // last node
} alloc_list_t;
```

---

## Global Variables

Each allocator file declares two lists as **static** globals — visible only within that file, not accessible from outside:

```c
static alloc_list_t allocated_list;   // chunks currently in use
static alloc_list_t free_list;        // chunks available for reuse
```

Quick fit uses five free lists (one per partition size) instead of one:

```c
static alloc_list_t allocated_list;
static alloc_list_t free_list[5];     // [0]=32, [1]=64, [2]=128, [3]=256, [4]=512
```

---

## Fixed Partition Sizes

When `alloc(n)` is called, `n` is rounded up to the nearest fixed size:

| Request range | Partition given |
|---------------|----------------|
| 1 – 32        | 32 bytes        |
| 33 – 64       | 64 bytes        |
| 65 – 128      | 128 bytes       |
| 129 – 256     | 256 bytes       |
| 257 – 512     | 512 bytes       |

Requests larger than 512 bytes cannot be served.

---

## `alloc()` Algorithm

1. Round `chunk_size` up to the nearest fixed partition size
2. Search the free list for a chunk of that exact size
3. **If found:** remove it from the free list, add it to the allocated list, return its address
4. **If not found:** call `sbrk(partition_size)` to get new memory from the OS, create a new `allocation_t`, add it to the allocated list, return its address
5. **If `sbrk()` fails:** return `NULL`

---

## `dealloc()` Algorithm

1. Search the allocated list for a node whose `space` matches the given pointer
2. **If found:** remove it from the allocated list, add it to the free list
3. **If not found:** print an error to `stderr` and terminate the program (`exit(1)`)

---

## List Operations (`utils/alloc_list.c`)

| Function | Description |
|----------|-------------|
| `push(list, alloc)` | Append an allocation to the tail of the list |
| `pop(list)` | Remove and return the allocation at the tail |
| `find_and_remove(list, space)` | Search by address, remove and return if found |
| `alloc_list_destroy(list)` | Free all nodes and allocations in the list |

---

## Output (printed at program end)

**Allocated list** — one line per chunk:
```
Address: <addr>  Total size: <total_size>  Used size: <used_size>
```

**Free list** — one line per chunk:
```
Address: <addr>  Total size: <total_size>
```

For quick fit, print one free list per partition size (32, 64, 128, 256, 512).
