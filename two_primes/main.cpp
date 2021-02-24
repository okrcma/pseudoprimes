#include <primesieve.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>


uint64_t fibonacci_mod(uint64_t n, uint64_t m) {
    __uint128_t x = 0;
    __uint128_t y = 1;

    __uint128_t tmp = n >> (uint) 1;
    uint64_t mask = 1;
    while (tmp != 0) {
        tmp >>= (uint) 1;
        mask <<= (uint) 1;
    }

    while (mask != 0) {
        tmp = x;
        x = x*(m+2*y-x) % m;
        y = (tmp * tmp + y * y) % m;

        if (n & mask) {
            tmp = x;
            x = y;
            y = (tmp + y) % m;
        }

        mask >>= (uint) 1;
    }

    return x;
}

uint64_t pow_mod(__uint128_t base, uint64_t exp, uint64_t mod) {
    __uint128_t res = 1;
    while (exp) {
        if (exp & (uint) 1)
            res = (base * res) % mod;
        base = (base * base) % mod;
        exp >>= (uint) 1;
    }

    return res;
}

bool admissible(uint64_t p, std::vector<uint64_t>& small_primes) {
    if (p % 5 != 1 && p % 5 != 4)
        return false;

    uint64_t t = 1; // rank(p) must divide t
    uint64_t n = p - 1; // p-1 = phi(p) = psi(p)

    uint64_t q = 2; // small prime
    uint64_t r = 1; // power of q dividing n

    // first test two
    while (n % q == 0) {
        n /= q;
        r *= q;
    }
    if (r != 1) {
        // test if order of 2 mod n is not divisible by power of two greater than 2**1
        if (pow_mod(2, (uint64_t) (p - 1) / (r/q), p) == 1)
            t *= r;
        else {
            t *= 2;
        }
    }

    // now test remaining small primes
    for (uint64_t* it = &small_primes[1];; it++) {
        q = *it; // small prime
        r = 1; // power of q dividing n
        while (n % q == 0) {
            n /= q;
            r *= q;
        }
        if (r != 1) {
            // test if order of 2 mod n is not divisible by q
            if (pow_mod(2, (uint64_t) (p - 1) / r, p) == 1)
                t *= r;
        }

        // test last prime dividing p-1
        if (q*q >= n) {
            if (n != 1)
                if (pow_mod(2, (uint64_t) (p - 1) / n, p) == 1)
                    t *= n;
            break;
        }
    }

    if (fibonacci_mod(t, p) != 0)
        return false;

    return true;
}

int main(int argc, char **argv) {
    // program takes two ints as arguments for the search bounds
    if (argc != 3)
        return 1;

    // get search bounds from arguments
    uint64_t lower_bound = std::stoll(argv[1]);
    uint64_t upper_bound = std::stoll(argv[2]);

    std::cout << "lower bound: " << lower_bound << std::endl;
    std::cout << "upper bound: " << upper_bound << std::endl;

    // start clock
    auto start = std::chrono::steady_clock::now();

    // vector of resulting primes
    std::vector<uint64_t> results;

    // get small primes
    std::vector<uint64_t> small_primes;
    primesieve::generate_primes(0, ceil(sqrt(upper_bound)), &small_primes);

    // iterator over primes to be checked
    primesieve::iterator it;
    it.skipto(lower_bound, upper_bound);

    // check each prime
    uint64_t prime;
    while ((prime = it.next_prime()) < upper_bound) {
        if (admissible(prime, small_primes)) {
            std::cout << "found: " << prime << std::endl;
            results.push_back(prime);
        }
    }

    // stop clock
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
    std::cout << "duration (seconds): " << duration << std::endl;

    // write results to file
    std::ofstream f ("results.txt", std::ios::out | std::ios::trunc);
    f << "lower bound: " << lower_bound << std::endl;
    f << "upper bound: " << upper_bound << std::endl;
    for (uint64_t p: results) {
        f << "found: " << p << std::endl;
    }
    f << "duration (seconds): " << duration << std::endl;
    f.close();

    return 0;
}
