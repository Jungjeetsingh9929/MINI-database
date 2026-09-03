# MiniDB From Scratch

MiniDB is a 7-day C++ learning project for building a tiny database engine from first principles.

This branch is **Day 2**.

## Day 2 Goal

Learn how a database stores structured records:

- Rows are converted into bytes before going to disk.
- Disk is split into fixed-size pages.
- Each page stores many fixed-size row slots.
- `insert` appends a student row that survives restart.

## Build

```bash
cmake -S . -B build
cmake --build build
```

If CMake is not installed, you can still compile this day directly:

```bash
c++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o minidb
```

## Run

```bash
./build/minidb students.db
```

Direct-compile version:

```bash
./minidb students.db
```

## Try

```text
MiniDB > insert 101 Jungjeet
MiniDB > insert 102 Rahul
MiniDB > .rows
MiniDB > .exit
```

Run the program again with the same `students.db` file and type `.rows`. Your students will still be there.

## Suggested Commit Message

```text
Day 2: store serialized rows in fixed-size pages
```
