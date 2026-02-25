# Hash Table Implementation (C++)

## Overview
This project implements a hash table using open addressing to store key-value pairs.  
Keys are hexadecimal strings and values are non-empty strings.

The hash table supports configurable hash functions and collision resolution strategies.

---

## My Contribution

I fully implemented:

- `hash_202.cpp` — Core hash table logic, including:
  - Hash function implementation
  - Collision resolution (linear probing / double hashing)
  - Insert and lookup operations
  - Key validation and storage handling

---

## Provided / Starter Files

The following files were provided as part of the assignment framework:

- `hash_tester.cpp`
- `read_as_hex.cpp`
- `hash_202.hpp`
- Build files (e.g., Makefile)

These were included to support testing and validation of the hash table implementation.

---

## Features
- Open addressing collision handling
- Custom hash function logic
- Configurable probing strategies
- Efficient key lookup and insertion
- Input validation for hexadecimal keys

---

## Concepts Used
- Hash tables
- Open addressing
- Linear probing / double hashing
- Modular arithmetic
- Vectors and dynamic memory
- Algorithmic collision resolution

---

## How to Compile

If using a Makefile:

```bash
make
