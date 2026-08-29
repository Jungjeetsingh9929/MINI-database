# MiniDB From Scratch

MiniDB is a 7-day C++ learning project for building a tiny database engine from first principles.

This branch is **Day 1**.

## Day 1 Goal

Learn how a database starts:

- A command-line shell accepts user commands.
- A database file is opened on disk.
- Data written to the file still exists after restarting the program.
- The difference between RAM and disk persistence becomes visible.

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
MiniDB > .help
MiniDB > write first persistent note
MiniDB > write database file survives restart
MiniDB > read
MiniDB > .stats
MiniDB > .exit
```

Run the program again with the same `students.db` file and type `read`. Your notes will still be there.

## Suggested Commit Message

```text
Day 1: add CLI shell and persistent file I/O
```

