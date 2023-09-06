def main(y):
    if (y < 63):
        return (9 * y ** 2 + 66) ** 7
    elif (y < 154):
        return 88 * y ** 5 - abs(y - y ** 2 - 93) ** 4
    elif (y < 177):
        return 42 * y ** 7
    return (3 * y) ** 3
