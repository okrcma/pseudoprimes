## case 1

This program tests integers (for passing the PSW-challenge test) in the form 2<sup>i</sup>-1 for all i within the given bounds.
Outputs the i for which the integers passed (the integers may be primes).

The code uses the NTL library (https://libntl.org/).

Compiling
```
gcc case1.cpp -g3 -pthread -lstdc++ -lntl -lgmp -lm
```

Running the program takes two arguments: the lower and upper bound for i. 
The output is printed to std::out and results.txt.

Example
```
./a.out 0 10000
lower bound: 2
upper bound: 10000
found: 2
found: 3
found: 7
found: 19
found: 31
found: 107
found: 127
found: 607
found: 1279
found: 2203
found: 4423
duration (seconds): 967

```

There are no passing integers for i from 10000 to 40000.

## case 2

This program looks for admissible primes whose rank is power of two.

The code uses primesieve library (https://github.com/kimwalisch/primesieve) to find primes. 

Compiling
```gcc case2.cpp -g3 -static -pthread -L. -l:libprimesieve.a -lstdc++ -lm```

Running the program takes two arguments: the lower and upper bound for the primes. The output is printed to std::out.
Each result is on a separate line. The first number is the prime and the second number is its rank.

Example
```
./a.out 0 10000000
3 4
7 8
47 16
127 128
1087 64
2207 32
4481 64
21503 1024
34303 512
119809 256
524287 524288
```

There are two more results up to 2<sup>30</sup>.
```
65241089 8192
167772161 16777216
```

These primes are not necessarily admissible.