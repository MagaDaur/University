import math


def main(a, b, m, y):
    sum_1 = 0
    for j in range(1, b + 1):
        sum_2 = 0
        for c in range(1, a + 1):
            sum_2 += (c ** 3 + 55 * math.log2(j ** 3 + c) ** 4)
        sum_1 += sum_2

    sum_3 = 0
    for k in range(1, m + 1):
        sum_4 = 0
        for i in range(1, a + 1):
            sum_4 += (74 * (1 - 19 * i ** 3 - 68 * y) ** 4 -
                      math.log(k) ** 2 - math.log10(k) ** 3)
        sum_3 += sum_4

    return sum_1 + sum_3
