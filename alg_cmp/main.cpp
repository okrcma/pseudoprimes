#include <iostream>
#include "fibonacci.hpp"
#include <chrono>

int main(int argc, char **argv) {
    // program takes three ints as arguments: number of algorithm, min n and max n
    if (argc != 4) {
        std::cout << "Wrong number of arguments." << std::endl;
        return 1;
    }

    // get arguments
    uint64_t k = std::stoll(argv[1]);
    uint64_t min_n = std::stoll(argv[2]);
    uint64_t max_n = std::stoll(argv[3]);

    // 1-4 are non-modular versions, 5-8 are modular versions
    NTL::ZZ(*alg)(uint64_t);
    switch (k) {
        case 1:
            alg = fibonacci1;
            break;
        case 2:
            alg = fibonacci2;
            break;
        case 3:
            alg = fibonacci3;
            break;
        case 4:
            alg = fibonacci4;
            break;
        case 5:
            alg = [](uint64_t n) { return fibonacci_mod1(n, n); };
            break;
        case 6:
            alg = [](uint64_t n) { return fibonacci_mod2(n, n); };
            break;
        case 7:
            alg = [](uint64_t n) { return fibonacci_mod3(n, n); };
            break;
        case 8:
            alg = [](uint64_t n) { return fibonacci_mod4(n, n); };
            break;
        default:
            std::cout << "There's no algorithm numbered " << k << "." << std::endl;
            return 1;
    }

    if (k <= 4)
        std::cout << "Running algorithm " << k << " from " << min_n << " up to " << max_n << "." << std::endl;
    else
        std::cout << "Running modular algorithm " << k - 4 << " from " << min_n << " up to " << max_n << "." << std::endl;

    // start clock
    auto start = std::chrono::steady_clock::now();

    uint64_t c = 0;
    for (uint64_t n = min_n; n <= max_n; n++) {
        // for modular version ignore n divisible by 5
        if (k >= 4 && n % 5 == 0)
            continue;
        if (alg(n) < 0) {
            c++; // to make sure the computation doesn't get optimized out
        }
    }

    // stop clock
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "duration (milliseconds): " << duration << std::endl;

    return c;
}
