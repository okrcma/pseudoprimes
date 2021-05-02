#include <iostream>
#include <NTL/ZZ.h>
#include <fstream>

uint64_t msb(uint64_t n) {
    if (n == 0)
        return 0;

    uint64_t tmp;
    tmp = n >> (uint) 1;

    uint64_t mask = 1;
    while (tmp != 0) {
        tmp >>= (uint) 1;
        mask <<= (uint) 1;
    }

    return mask;
}

NTL::ZZ fibonacci(uint64_t n) {
    NTL::ZZ V_l;
    V_l = 2;
    NTL::ZZ V_h;
    V_h = 1;

    NTL::ZZ Q_l;
    Q_l = 1;
    NTL::ZZ Q_h;
    Q_h = 1;

    uint64_t mask = msb(n);
    while (mask != 0) {
        Q_l = Q_l * Q_h;
        if (n & mask) {
            Q_h = -Q_l;
            V_l = V_h * V_l - Q_l;
            V_h = V_h * V_h - 2 * Q_h;
        }
        else {
            Q_h = Q_l;
            V_h = V_h * V_l - Q_l;
            V_l = V_l * V_l - 2 * Q_h;
        }

        mask >>= (uint) 1;
    }

    return (2 * V_h - V_l) / 5;
}

int main() {
    std::ofstream outfile("case2_output.txt", std::ios::app);
    std::ifstream infile("case2_input.txt");

    long k;
    while (infile >> k) {
        std::cout << k << std::endl;

        NTL::ZZ x;
        x = NTL::power2_ZZ(k-1)-1;
        NTL::ZZ y;
        y = fibonacci(k+1);
        NTL::ZZ res;
        res = NTL::GCD(x, y);

        outfile << k << " " << res << std::endl;

        std::cout << res << std::endl;
        std::cout << std::endl;
    }

    infile.close();
    outfile.close();

    return 0;
}
