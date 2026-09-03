# Day 1 Concepts: CLI + File Persistence

## What Is a Database File?

A database is not magic storage. At the lowest level, it is usually one or more files on disk.

When a program stores data only in variables, that data disappears when the program exits. When it writes bytes into a file, the operating system can keep those bytes after the program stops.

## RAM vs Disk

RAM:

- Fast
- Temporary
- Cleared when the process exits

Disk:

- Slower than RAM
- Persistent
- Stores bytes that can be read later

## What We Build Today

Today's MiniDB supports a tiny shell:

```text
MiniDB > write hello
MiniDB > read
MiniDB > .stats
MiniDB > .exit
```

This is not SQL yet. That is intentional. The first goal is to see persistence clearly.

## Mental Model

```text
User command
   |
   v
MiniDB shell
   |
   v
Database file on disk
```

## Interview Questions

1. Why does data disappear when stored only in normal variables?
2. What does persistence mean?
3. Why do database systems need files?
4. Why is a command-line shell useful when building a database from scratch?

