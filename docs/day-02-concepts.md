# Day 2 Concepts: Pages, Rows, Serialization

## Why Pages?

Databases usually do not write one tiny record at a time as plain text. They group bytes into fixed-size blocks called pages.

MiniDB uses 4096-byte pages. This matches the mental model used by many real storage systems.

## Why Serialize?

A C++ struct exists in memory. A file stores bytes. Serialization means converting a row into a predictable byte layout so it can be written to disk and reconstructed later.

## Row Layout

MiniDB's Day 2 student row is fixed width:

```text
active flag: 1 byte
id:          4 bytes
name:        32 bytes
```

Fixed-width rows are simpler to learn because every slot has the same size.

## Insert Flow

```text
insert 101 Jungjeet
   |
   v
parse id + name
   |
   v
serialize row into bytes
   |
   v
write bytes at next row slot
```

## Interview Questions

1. What problem does serialization solve?
2. Why do databases group disk data into pages?
3. What is the trade-off of fixed-size text fields?
4. Why is page size usually much larger than one row?

