"""
This module finds all admissible products of small primes up to a given bound (at most 2**11).
The computation is split into stages. In each stage the current admissible products are multiplied
by the primes which gives us new admissible products. The admissible products are checked for being PSW-challenge
pseudoprimes.
"""

from math import gcd

# primes < 2**10
small_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101,
                103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
                223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337,
                347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461,
                463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
                607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739,
                743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021,
                1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129,
                1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277,
                1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409,
                1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
                1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621,
                1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759,
                1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901,
                1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029,
                2039]


def factor(n):
    """
    Factor n by trial division. Uses the small_primes list.
    :param n: Number to be factored. It's second largest prime divisor has to be in small_primes.
    :return: List [(q, e)] where q is a prime and q**e is the maximal power dividing n.
    """

    result = {}
    for p in small_primes:
        if p ** 2 > n:
            break
        while n % p == 0:
            e = result.setdefault(p, 0)
            result[p] = e + 1
            n //= p

    if n != 1:
        result[n] = 1

    return list(result.items())


def lcm(a, b):
    """
    Compute least common multiple.
    """
    return abs(a * b) // gcd(a, b)


def phi(n):
    """
    Compute Euler phi of n.
    """
    factors = factor(n)

    t = 1
    for q, e in factors:
        t *= q ** (e - 1) * (q - 1)

    return t


def order(n):
    """
    Compute order of 2 modulo n.
    :param n: Positive integer not divisible by 2.
    """
    assert n % 2 == 1

    t = phi(n)
    factors = factor(t)

    for p, e in factors:
        t = t // p ** e
        x = pow(2, t, n)
        while x != 1:
            t = t * p
            x = pow(x, p, n)

    return t


def lucas_mod(n, mod):
    """
    Compute n-th element of the Fibonacci sequence.
    """
    x, y = 0, 1  # U_n, U_{n+1}, n=0
    for b in bin(n)[2:]:
        x, y = ((y - x) * x + x * y) % mod, (x * x + y * y) % mod  # double
        if b == "1":
            x, y = y, (x + y) % mod  # add
    return x


def epsilon(n):
    """
    Compute Jacobi symbol (5/n).
    """
    if n % 5 in [1, 4]:
        return 1
    elif n % 5 in [2, 3]:
        return -1
    else:
        return 0


def psi(n):
    """
    Compute the psi function for n.
    """
    factors = factor(n)

    t = 1
    for q, e in factors:
        if q == 2:
            t *= q ** (e - 1) * (q + 1)
        else:
            t *= q ** (e - 1) * (q - epsilon(q))

    return t


def rank(n):
    """
    Compute the rank of appearance of n in the Fibonacci sequence.
    """
    t = psi(n)
    factors = factor(t)

    for q, e in factors:
        t = t // q ** e
        x = lucas_mod(t, n)
        while x != 0:
            t = t * q
            x = lucas_mod(t, n)

    return t


def admissible(x):
    k, o, r = x
    return gcd(o, r) <= 2


if __name__ == "__main__":
    # upper bound for the primes
    bound = 2 ** 10
    assert bound <= 2 ** 11

    # stage 0
    primes = [p for p in small_primes if 2 < p <= bound]
    primes = [(p, order(p), rank(p)) for p in primes]
    primes = [p for p in primes if admissible(p)]
    products = set(x for x in primes)

    print("stage", 0)
    print("number of new products", len(products))
    print()

    # maximum product
    mp = max(n for n, _, _ in products)

    # stage i
    i = 1
    while len(products) > 0:
        print("stage", i)

        # compute new products
        products = set(
            (x1 * x2, lcm(o1, o2), lcm(r1, r2))
            for x1, o1, r1 in products
            for x2, o2, r2 in primes
            if x1 % x2 != 0
            and gcd(o1, r2) <= 2 and gcd(o2, r1) <= 2
        )

        # test the products for being challenge pseudoprime
        for n, o, r in products:
            if (n - 1) % o == 0 and (n + 1) % r == 0:
                if n not in small_primes:
                    print("hurray", n)

        if len(products) > 0:
            mp = max(mp, max(n for n, _, _ in products))

        print("number of new products", len(products))
        print()
        i += 1

    print("maximum product:", mp)
