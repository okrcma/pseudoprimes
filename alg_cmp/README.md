This package contains implementations of the four algorithms for computing
the Fibonacci sequence.

The algorithms and their modular versions are implemented in ```fibonacci.hpp```.
The ```main.cpp``` program measures speed of computation of these algorithms. It takes three arguments:
number of algorithm, lower bound, upper bound and runs the chosen algorithm for all values between the bounds.
Algorithms 5-8 are modular versions of algorithms 1-4. For modular versions, the program computes U<sub>n</sub> mod n for
all n not divisible by 5 between the bounds.

Algorithms 1 and 5 are the slowest. Algorithm 2 and 6 are slightly faster than the rest,
however algorithm 6 cannot compute U<sub>k</sub> mod n where k, n are not coprime.

The code uses the NTL library (https://libntl.org/).

Compiling
```
gcc main.cpp -pthread -lstdc++ -lntl -lgmp -lm 
```

Example
```
./a.out 1 0 10000
Running algorithm 1 from 0 up to 10000.
duration (milliseconds): 101
```