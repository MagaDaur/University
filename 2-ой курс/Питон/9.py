import re
import math


def remove_none(x):
    res = []
    for row in x:
        temp = []
        for val in row:
            if val is None:
                continue
            temp.append(val)
        if len(temp):
            res.append(temp)

    return res


def remove_duplicates(x):
    res = []
    for row in x:
        temp = []
        for item in row:
            if item in temp:
                continue
            temp.append(item)
        if temp in res:
            continue
        res.append(temp)
    return res


def format(x):
    res = []
    for row in x:
        temp = []
        temp.append(str(round(float(row[0].replace('%', '')) / 100, 1)))
        temp.append(row[1].split(' ')[1])
        temp.append(row[2].lower())
        res.append(temp)
    return res


def transpose(x):
    res = [[], [], []]
    for row in x:
        res[0].append(row[0])
        res[1].append(row[1])
        res[2].append(row[2])

    return res


def main(x):
    res = remove_none(x)
    res = remove_duplicates(res)
    res = format(res)
    res = transpose(res)
    return res
