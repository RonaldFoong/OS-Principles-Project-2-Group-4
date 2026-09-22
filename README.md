# Project 2 — Memory Allocation

**Course:** Operating Systems Principles, RMIT  
**Team:** Project 2 Group 4 
**Members:** S4204551 · S4197581 · S4000196 · S4055262  
**Due:** Monday, October 5th, 2026, at 23:59

---

## Task Completion Status

Update this table as work progresses. The README must state exactly which tasks and subtasks have been completed.

| Task | Status |
|---|---|
| Task 1 — Solution Design | ✅ Completed |
| Task 2 – Memory Allocation | ✅ Completed |
| Task 3– Allocation Strategies | ✅ Completed |
| Task 4– Memory Deallocation | ✅ Completed |

### Status Indicators

- ⬜ Not started
- 🔶 In progress
- ✅ Completed

---

## Build Instructions

This project can be compiled and run on any of the RMIT Core Teaching Servers:

- `jupiter.csit.rmit.edu.au`
- `saturn.csit.rmit.edu.au`
- `titan.csit.rmit.edu.au`

### Download and Compile

```bash
ssh <user>@<rmit-server>
wget https://github.com/RonaldFoong/OS-Principles-Project-2-Group-4/archive/refs/tags/submission.zip
unzip submission.zip
cd OS-Principles-Project-2-Group-4-submission
make all
```

This builds `firstfit`, `bestfit`, and `quickfit`.

---

## Running the Programs

```bash
./bin/<program> <datafile>
```

Allocates and deallocates chunks of memory according to instructions in the datafile, and prints the state of the allocated list and free list.

**Example:**

```bash
./bin/firsfit datafile.txt
```

---

### Extra Makefile Functions

#### Clean Up Compiled Files

```bash
make clean
```

---

## Memory Testing

Run Valgrind using:

```bash
valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./bin/<program> <datafile>
```

Run Leaks using:

```bash
leaks -atExit -- ./bin/<program> <datafile>
```
