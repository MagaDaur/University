import re


def main(x):
    r = r'auto\s*@\'([^\']+)\'\s*::=\s*#\s*([^\.]+)\.'
    z = re.findall(r, x)
    res = {}
    for name, number in z:
        res[name] = int(number)
    return res