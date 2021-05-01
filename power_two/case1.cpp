#include <iostream>
#include <NTL/ZZ.h>
#include <chrono>
#include <vector>
#include <fstream>


/**
 * Compute fibonacci number for 2**i mod m. Using double and add method.
 */
NTL::ZZ fibonacci_mod(int i, const NTL::ZZ& m) {
    NTL::ZZ x;
    x = 0;
    NTL::ZZ y;
    y = 1;

    NTL::ZZ tmp;

    // add for the first 1
    tmp = x;
    x = y;
    y = (tmp + y) % m;

    while (true) {
        // double for each 0
        tmp = x;
        x = x*(2*y-x) % m;
        y = (tmp * tmp + y * y) % m;

        if (i == 1) {
            return x;
        }
        i -= 1;
    }
}

int main(int argc, char **argv) {
    // program takes two ints as arguments for the search bounds
    if (argc != 3)
        return 1;

    // get search bounds from arguments
    uint lower_bound = std::stoll(argv[1]);
    uint upper_bound = std::stoll(argv[2]);

    if (lower_bound < 2) {
        lower_bound = 2;
    }

    std::cout << "lower bound: " << lower_bound << std::endl;
    std::cout << "upper bound: " << upper_bound << std::endl;

    // start clock
    auto start = std::chrono::steady_clock::now();

    // vector of resulting pseudoprimes
    std::vector<NTL::ZZ> results;

    NTL::ZZ two;
    two = 2;

    for (uint i = lower_bound; i <= upper_bound; i++) {
        NTL::ZZ n; // n = 2**i-1
        n = 2;
        n = n << i-1;
        n -= 1;

        NTL::ZZ tmp;

        // fermat test
        tmp = NTL::PowerMod(two, n - 1, n);
        if (tmp != 1) {
            continue;
        }

        // lucas test
        tmp = fibonacci_mod(i, n);
        if (tmp != 0) {
            continue;
        }

        std::cout << "found: " << i << std::endl;
        results.push_back(n);
    }

    // stop clock
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
    std::cout << "duration (seconds): " << duration << std::endl;

    // write results to file
    std::ofstream f ("results.txt", std::ios::out | std::ios::trunc);
    f << "lower bound: " << lower_bound << std::endl;
    f << "upper bound: " << upper_bound << std::endl;
    for (const NTL::ZZ& p: results) {
        f << "found: " << p << std::endl;
    }
    f << "duration (seconds): " << duration << std::endl;
    f.close();

    return 0;
}
