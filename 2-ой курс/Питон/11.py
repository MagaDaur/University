from struct import *
import json

FMT = dict(
    char='c',
    int8='b',
    uint8='B',
    int16='h',
    uint16='H',
    int32='i',
    uint32='I',
    int64='q',
    uint64='Q',
    float='f',
    double='d'
)


def parse(buf, offs, ty, order='>'):
    pattern = FMT[ty]
    size = calcsize(pattern)
    value = unpack_from(order + pattern, buf, offs)[0]
    return value, offs + size


def parse_d(buf, offs):
    d1 = []
    for _ in range(2):
        d1_i, offs = parse(buf, offs, 'uint64')
        d1.append(d1_i)
    d2, offs = parse(buf, offs, 'uint32')
    d3, offs = parse(buf, offs, 'int64')
    return dict(D1=d1, D2=d2, D3=d3), offs


def parse_c(buf, offs):
    c1, offs = parse(buf, offs, 'uint16')
    c2, offs = parse(buf, offs, 'float')
    return dict(C1=c1, C2=c2), offs


def parse_b(buf, offs):
    b1, offs = parse(buf, offs, 'double')
    b2 = []
    for _ in range(7):
        b2_offs, offs = parse(buf, offs, 'uint32')
        b2_i, _ = parse_c(buf, b2_offs)
        b2.append(b2_i)
    b3_size, offs = parse(buf, offs, 'uint32')
    b3_start, offs = parse(buf, offs, 'uint16')
    b3 = []
    for _ in range(b3_size):
        b3_i, b3_start = parse(buf, b3_start, 'int8')
        b3.append(b3_i)
    b4, offs = parse(buf, offs, 'double')
    b5, offs = parse_d(buf, offs)
    b6, offs = parse(buf, offs, 'uint8')
    b7, offs = parse(buf, offs, 'uint32')

    return dict(B1=b1, B2=b2, B3=b3, B4=b4, B5=b5, B6=b6, B7=b7), offs


def parse_a(buf, offs):
    a1, offs = parse(buf, offs, 'uint16')
    a2 = []
    for _ in range(2):
        a2_i, offs = parse(buf, offs, 'char')
        a2.append(a2_i.decode('utf-8'))
    a2 = ''.join(a2)
    a3, offs = parse_b(buf, offs)
    a4, offs = parse(buf, offs, 'uint64')
    a5, offs = parse(buf, offs, 'uint32')
    a6, offs = parse(buf, offs, 'uint32')
    a7, offs = parse(buf, offs, 'uint32')

    return dict(A1=a1, A2=a2, A3=a3, A4=a4, A5=a5, A6=a6, A7=a7), offs


def main(stream):
    return parse_a(stream, 5)[0]
