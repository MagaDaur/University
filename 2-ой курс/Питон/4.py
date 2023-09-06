import math


def main(n):
    if (n == 0):
        return 0.35
    elif (n == 1):
        return 0.45
    return math.tan(main(n - 2)) ** 3\
        + main(n - 1)\
        + 22 * (main(n - 2) ** 3 / 73) ** 2
