import math


def main(y, z):
    return 37 * (41 * y ** 3 - z ** 2) ** 7 + 1\
        + (math.acos(z) ** 3 - (92 - z - 96 * y ** 3) ** 7)\
        / (53 * z ** 2 - (36 * z - 0.01 - 50 * y ** 3))
