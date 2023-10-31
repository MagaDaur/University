a = input()
b = input()
c = [int(i) for i in (a + b)]
d = max(c)
e = min(c)
c.remove(d)
c.remove(e)
c.insert(1, sum(c) % 10)
c[0] = d
c[2] = e
print(f'{c[0]}{c[1]}{c[2]}')