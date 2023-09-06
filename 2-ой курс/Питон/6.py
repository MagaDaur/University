def one(x, left, middle, right):
    if x[1] == 'RAGEL':
        return left
    if x[1] == 'LEX':
        return middle
    return right


def two(x, left, middle, right):
    if x[2] == 'APL':
        return left
    if x[2] == 'NSIS':
        return middle
    return right


def three(x, left, right):
    if x[3] == 2002:
        return left
    return right


def four(x, left, middle, right):
    if x[4] == 1979:
        return left
    if x[4] == 1970:
        return middle
    return right


def main(x):
    if x[0] == 1971:
        return three(x, four(x, two(x, 0, 1, 2), 3, 4), 5)
    if x[0] == 1992:
        return four(x, 6, one(x, three(x, 7, 8), three(x, 9, 10), 11), 12)
    return 13

assert main([1975, 'LEX', 'NSIS', 1984, 1970]) == 13
assert main([1971, 'LEX', 'NSIS', 2002, 1979]) == 1
assert main([1971, 'RAGEL', 'NIM', 2002, 1996]) == 4
assert main([1971, 'LEX', 'NSIS', 1984, 1996]) == 5
assert main([1992, 'GOLO', 'NIM', 1984, 1979]) == 6