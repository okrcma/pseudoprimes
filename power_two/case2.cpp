#include <iostream>
#include <vector>
#include <primesieve.hpp>


int epsilon(uint64_t n) {
    if (n % 5 == 0)
        return 0;
    else if (n % 5 == 2 || n % 5 == 3)
        return -1;
    else
        return 1;
}

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

uint64_t rank_prime(uint64_t p, std::vector<uint64_t>& small_primes) {
    uint64_t n = p - epsilon(p);
    uint64_t t = n;

    uint64_t q;

    for (uint64_t* it = &small_primes[0];; it++) {
        q = *it; // small prime
        while (n % q == 0) {
            n /= q;
            t /= q;
        }
        uint64_t x = fibonacci_mod(t, p);
        while (x != 0) {
            t *= q;
            x = fibonacci_mod(t, p);
        }

        if (q*q >= n) {
            if (n != 1 && fibonacci_mod(t/n, p) == 0)
                return t/n;
            return t;
        }
    }
}

bool is_power_of_two(uint64_t n) {
    while (n > 1) {
        if (n & 1)
            return false;
        n >>= 1;
    }
    return true;
}

int main(int argc, char **argv) {
    // program takes two ints as arguments for the search bounds
    if (argc != 3)
        return 1;

    // get search bounds from arguments
    uint64_t lower_bound = std::stoll(argv[1]);
    uint64_t upper_bound = std::stoll(argv[2]);

    std::vector<uint64_t> small_primes;
    primesieve::generate_primes(0, ceil(sqrt(upper_bound)), &small_primes);

    primesieve::iterator it;
    it.skipto(lower_bound, upper_bound);
    uint64_t prime;
    while ((prime = it.next_prime()) < upper_bound) {
        uint64_t rank = rank_prime(prime, small_primes);
        if (is_power_of_two(rank))
            std::cout << prime << " " << rank_prime(prime, small_primes) << std::endl;
    }

    return 0;
}
