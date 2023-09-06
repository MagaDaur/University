import math


def main(y, x, z):
    sum_1 = 0
    n = len(y) + 1
    for i in range(1, n):
        sum_1 += math.tan(68 * z[math.ceil(i / 4) - 1]
                          + 63 * x[n - math.ceil(i / 4) - 1] ** 3
                          + y[i - 1] ** 2)

    return 60 * sum_1
