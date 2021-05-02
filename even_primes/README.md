## case 1

The program ```case1.py``` searches for k such that n=pk is PSW-challenge pseudoprime.
The prime p is one of the ten known admissible primes with epsilon=1 (by default it's 61681).
The program takes two inputs as bounds and searches for k such that n is between these bounds.

Example
```
$ python3 case1.py 0 1125899906842624
searching for k for prime 61681 with order 40 and rank 1542 such that n=pk is in range from 0 to 1125899906842624
k % m = a, where a=16961, m=30840
initial k: 16961
last k 18253596161
checked 591880 possibilities for k
elapsed time (seconds): 0.3592703342437744
```

## case 2

The program ```case2.cpp``` reads integers from ```case2_input.txt```
line by line and computes the greatest common divisor of their order and rank.
It outputs the result to ```case2_output.txt```.

The code uses the NTL library (https://libntl.org/).

Compiling
```
gcc case2.cpp -g3 -pthread -lntl -lm -lstdc++ -lgmp
```

The file ```case2_input.txt``` contains all composite admissible integers
with epsilon=-1 less than 2<sup>25</sup>.