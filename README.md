# Parallel Prime Number Sieve

This program implements a parallel programming version of the Sieve of Eratosthenes algorithm to find prime numbers up to a specified maximum value using the constant `MAX`. It utilizes multiple threads to improve performance on multi-core systems.

## Algorithm Overview

The Sieve of Eratosthenes is an ancient algorithm for finding all prime numbers up to a given limit. This implementation uses the algorithm with parallelization.

[Algorithm Explanation](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)

## Program

- Uses a bit-packed representation of the sieve array to save memory.
- Uses multithreading to parallelize the sieve computation.

## Usage

Compile the program and run it to find all prime numbers up to `MAX`.

```bash
gcc -o sieve Sieve.c -lpthread -lm
./sieve
```

The program will output a truncated list of prime numbers to the console and the complete list to a file named `primes_output.txt`.