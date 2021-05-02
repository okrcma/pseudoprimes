import sys
from math import gcd
import time


def lcm(a, b):
    """
    Compute least common multiple.
    """
    return abs(a * b) // gcd(a, b)


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


def lucas_mod(n, mod):
    """
    Compute n-th element of Fibonacci sequence modulo mod.
    """
    P, Q = 1, -1
    x, y = 0, 1  # U_n, U_{n+1}, n=0
    for b in bin(n)[2:]:
        x, y = ((y - P * x) * x + x * y) % mod, (-Q * x * x + y * y) % mod  # double
        if b == "1":
            x, y = y, (-Q * x + P * y) % mod  # add
    return x


def bezout(a, b):
    """
    Compute Bezout's coefficients.
    """
    r2, r1 = a, b
    s2, s1 = 1, 0
    t2, t1 = 0, 1

    while r1 != 0:
        q = r2 // r1
        r2, r1 = r1, r2 - q * r1
        s2, s1 = s1, s2 - q * s1
        t2, t1 = t1, t2 - q * t1

    return s2, t2


def crt(p, o, r):
    """
    Compute s such that s mod o == 1 and s mod r == -epsilon(p). Only for admissible primes p.
    :param p: admissible prime
    :param o: order of p
    :param r: rank of p
    :return: (s, lcm(o, r))
    """
    g = gcd(o, r)
    assert g <= 2

    if o % 4 != 0:
        o //= g
    else:
        r //= g

    a, b = bezout(o, r)
    s = b * r - a * o

    m = o * r

    if epsilon(p) == -1:
        return 1, m

    while s < 0:
        s += m
    return s, m


primes = [
    (61681, 40, 1542),
    (363101449, 171436, 1059),
    (4278255361, 80, 6684774),
    (4562284561, 120, 147934),
    (4582537681, 160453, 1428),
    (26509131221, 748, 14176006),
    (422013019339, 290442546, 2906),
    (2847994170041, 806587, 1765460),
    (10052678938039, 69, 145690999102),
    (20474359556069, 145084, 141120727),
]

if __name__ == "__main__":
    # get bounds from arguments
    args = sys.argv
    if len(args) != 3:
        print("expected two bound arguments")
        exit(1)
    lower_bound = int(args[1])
    upper_bound = int(args[2])

    # set the prime to be checked
    p, o, r = primes[0]

    print(
        "searching for k for prime {} with order {} and rank {} such that n=pk is in range from {} to {}".format(
            p, o, r, lower_bound, upper_bound
        )
    )

    # start time
    start_time = time.time()

    # compute the a = k % m by CRT
    a, m = crt(p, o, r)
    assert m == lcm(o, r)
    assert a % o == 1
    assert a % r == (r - epsilon(p)) % r
    print("k % m = a, where a={}, m={}".format(a, m))

    # initial value for k
    k = a + m*(lower_bound//m//p-1)
    while k < 0:
        k += m
    print("initial k:", k)

    # search for suitable k
    c = 0  # number of try values for k
    n = p * k
    while n <= upper_bound:
        if epsilon(n) == -1 and pow(2, n - 1, n) == 1 and lucas_mod(n + 1, n) == 0:
            print("hurray", n)
        k += m
        n = p * k
        c += 1
    print("last k", k)

    print("checked {} possibilities for k".format(c))

    # elapsed time
    end_time = time.time()
    print("elapsed time (seconds):", end_time-start_time)
