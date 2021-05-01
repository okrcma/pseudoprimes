#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>

/**
 * Get most significant byte of n.
 * @param n
 * @return only the most significant byte of n set
 */
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

/**
 * Algorithm by Marc Joye and Jean-Jacques Quisquater in Efficient computation of full Lucas sequences.
 */
NTL::ZZ fibonacci1(uint64_t n) {
    if (n == 0)
        return (NTL::ZZ) 0;

    NTL::ZZ Uh;
    Uh = 1;
    NTL::ZZ Vl;
    Vl = 2;
    NTL::ZZ Vh;
    Vh = 1;
    NTL::ZZ Ql;
    Ql = 1;
    NTL::ZZ Qh;
    Qh = 1;

    uint64_t s = 0;
    while (n % 2 == 0) {
        n /= 2;
        s++;
    }

    uint64_t mask = msb(n);
    while (mask != 1) {
        Ql = Ql * Qh;
        if (n & mask) {
            Qh = -Ql;
            Uh = Uh * Vh;
            Vl = Vh * Vl - Ql;
            Vh = Vh * Vh - 2*Qh; // mistake in the original paper
        }
        else {
            Qh = Ql;
            Uh = Uh * Vl - Ql;
            Vh = Vh * Vl - Ql;
            Vl = Vl * Vl - 2 * Ql;
        }

        mask >>= (uint) 1;
    }

    Ql = Ql * Qh;
    Qh = -Ql;
    Uh = Uh * Vl - Ql;
    Vl = Vh * Vl - Ql;
    Ql = Ql * Qh;

    for (uint64_t i = 1; i <= s; i++) {
        Uh = Uh * Vl;
        Vl = Vl * Vl - 2*Ql;
        Ql = Ql * Ql;
    }

    return Uh;
}

/**
 * Algorithm by Aleksey Koval in On Lucas Sequences Computation.
 */
NTL::ZZ fibonacci2(uint64_t n) {
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

/**
 * Algorithm derived from matrix multiplication.
 */
NTL::ZZ fibonacci3(uint64_t n) {
    NTL::ZZ x;
    x = 0;
    NTL::ZZ y;
    y = 1;

    NTL::ZZ tmp;

    uint64_t mask = msb(n);
    while (mask != 0) {
        tmp = x;
        x = x*(2*y-x);
        y = (tmp * tmp + y * y);

        if (n & mask) {
            tmp = x;
            x = y;
            y = (tmp + y);
        }

        mask >>= (uint) 1;
    }

    return x;
}

/**
 * Adjusted algorithm derived from matrix multiplication.
 */
NTL::ZZ fibonacci4(uint64_t n) {
    NTL::ZZ x;
    x = 0;
    NTL::ZZ y;
    y = 1;

    NTL::ZZ x2;
    NTL::ZZ y2;

    uint64_t mask = msb(n);
    while (mask != 0) {
        x2 = x*x;
        y2 = y*y;

        if (n & mask) {
            y = 2*x*y + y2;
            x = x2+y2;
        }
        else {
            x = 2*x*y - x2;
            y = x2+y2;
        }

        mask >>= (uint) 1;
    }

    return x;
}

/**
 * Modular version of algorithm by Marc Joye and Jean-Jacques Quisquater in Efficient computation of full Lucas sequences.
 */
NTL::ZZ fibonacci_mod1(uint64_t n, uint64_t m) {
    NTL::ZZ_p::init((NTL::ZZ) m);

    if (n == 0)
        return (NTL::ZZ) 0;

    NTL::ZZ_p Uh;
    Uh = 1;
    NTL::ZZ_p Vl;
    Vl = 2;
    NTL::ZZ_p Vh;
    Vh = 1;
    NTL::ZZ_p Ql;
    Ql = 1;
    NTL::ZZ_p Qh;
    Qh = 1;

    uint64_t s = 0;
    while (n % 2 == 0) {
        n /= 2;
        s++;
    }

    uint64_t mask = msb(n);
    while (mask != 1) {
        Ql = Ql * Qh;
        if (n & mask) {
            Qh = -Ql;
            Uh = Uh * Vh;
            Vl = Vh * Vl - Ql;
            Vh = Vh * Vh - 2*Qh; // mistake in the original paper
        }
        else {
            Qh = Ql;
            Uh = Uh * Vl - Ql;
            Vh = Vh * Vl - Ql;
            Vl = Vl * Vl - 2 * Ql;
        }

        mask >>= (uint) 1;
    }

    Ql = Ql * Qh;
    Qh = -Ql;
    Uh = Uh * Vl - Ql;
    Vl = Vh * Vl - Ql;
    Ql = Ql * Qh;

    for (uint64_t i = 1; i <= s; i++) {
        Uh = Uh * Vl;
        Vl = Vl * Vl - 2*Ql;
        Ql = Ql * Ql;
    }

    return rep(Uh);
}

/**
 * Modular version of algorithm by Aleksey Koval in On Lucas Sequences Computation.
 */
NTL::ZZ fibonacci_mod2(uint64_t n, uint64_t m) {
    NTL::ZZ_p::init((NTL::ZZ) m);

    NTL::ZZ_p V_l;
    V_l = 2;
    NTL::ZZ_p V_h;
    V_h = 1;

    NTL::ZZ_p Q_l;
    Q_l = 1;
    NTL::ZZ_p Q_h;
    Q_h = 1;

    uint64_t tmp;
    tmp = n >> (uint) 1;

    uint64_t mask = 1;
    while (tmp != 0) {
        tmp >>= (uint) 1;
        mask <<= (uint) 1;
    }

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

    return rep((2 * V_h - V_l) / 5);  // fails if 5 divides m
}

/**
 * Modular version of algorithm derived from matrix multiplication.
 */
NTL::ZZ fibonacci_mod3(uint64_t n, uint64_t m) {
    NTL::ZZ_p::init((NTL::ZZ) m);

    NTL::ZZ_p x;
    x = 0;
    NTL::ZZ_p y;
    y = 1;

    NTL::ZZ_p tmp;

    uint64_t mask = msb(n);
    while (mask != 0) {
        tmp = x;
        x = x*(2*y-x);
        y = (tmp * tmp + y * y);

        if (n & mask) {
            tmp = x;
            x = y;
            y = (tmp + y);
        }

        mask >>= (uint) 1;
    }

    return rep(x);
}

/**
 * Modular version of adjusted algorithm derived from matrix multiplication.
 */
NTL::ZZ fibonacci_mod4(uint64_t n, uint64_t m) {
    NTL::ZZ_p::init((NTL::ZZ) m);

    NTL::ZZ_p x;
    x = 0;
    NTL::ZZ_p y;
    y = 1;

    uint64_t tmp;
    tmp = n >> (uint) 1;

    uint64_t mask = 1;
    while (tmp != 0) {
        tmp >>= (uint) 1;
        mask <<= (uint) 1;
    }

    NTL::ZZ_p x2;
    NTL::ZZ_p y2;

    while (mask != 0) {
        x2 = x*x;
        y2 = y*y;

        if (n & mask) {
            y = 2*x*y + y2;
            x = x2+y2;
        }
        else {
            x = 2*x*y - x2;
            y = x2+y2;
        }

        mask >>= (uint) 1;
    }

    return rep(x);
}