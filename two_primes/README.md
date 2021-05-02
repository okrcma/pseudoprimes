This program looks for admissible primes p such that ϵ(p) = -1.

The code uses primesieve library (https://github.com/kimwalisch/primesieve) to find primes. 

Compiling
```
gcc main.cpp -g3 -lprimesieve -lstdc++ -lm
```

Running the program takes two arguments: the lower and upper bound for p. The maximum upper bound is limited to 2**63-1. 
The output is printed to std::out and results.txt.

Example
```
./a.out 0 1000000000
lower bound: 0
upper bound: 1000000000
found: 61681
found: 363101449
duration (seconds): 72

```

Results up to 2**45
```
61681
363101449
4278255361
4562284561
4582537681
26509131221
422013019339
2847994170041
10052678938039
20474359556069
```