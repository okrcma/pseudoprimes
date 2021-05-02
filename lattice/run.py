from sage.all import Zmod, Matrix, factor


def build_matrix(P, M, c=1000):
    factors_M = factor(M)
    rows = []

    # add logarithms
    for p in P:
        row = []
        for q, e in factors_M:
            row.extend(Zmod(q ** e)(p).generalised_log())  # generalised_log() uses unit_gens() generators
        row = [c * x for x in row]  # multiply logs by a large constant to help LLL
        rows.append(row)

    height = len(rows)
    width = len(rows[0])

    # add unit matrix
    for i, row in enumerate(rows):
        row.extend([1 if j == i else 0 for j in range(0, height)])

    # add group orders
    generators_M = [g for q, e in factors_M for g in Zmod(q ** e).unit_gens()]
    for i, g in enumerate(generators_M):
        rows.append([g.multiplicative_order() * c if j == i else 0 for j in range(0, width + height)])

    return Matrix(rows)


if __name__ == "__main__":
    M = 7550611589521 - 1
    P = [7, 11, 13, 17, 19, 31, 37, 41, 59, 61, 73, 97, 109, 151, 181, 233, 241, 257, 331, 349, 433, 523, 577, 631, 673,
         929, 1103, 1321, 1741, 2089, 4177, 5569, 6961, 7309, 8353, 9281, 13921, 17401, 23311, 29581, 38737, 41761,
         50993, 54001, 57367, 59393, 61681, 75169, 82129, 109621, 127481, 135721, 146161, 168781, 224461, 286831,
         458929, 501121, 509921, 605531, 680803, 789961, 1529761, 1733729, 2416861, 3033169, 3327229, 4334321, 4487297,
         4589281, 4818061, 4876111, 5446417, 7060051, 7553921, 8318071, 8369281]

    X = build_matrix(P, M)
    print(X)
    print("solving...")
    print(X.BKZ(block_size=40))  # the second row should contain the solution
