import numpy as np
alt = [
    [250000, 4.4, 144, 35],
    [200000, 4.9, 100, 20],
    [226000, 4.9, 100, 30],
    [230000, 4.5, 120, 100],
    [220000, 4.1, 130, 95],
    [180000, 4.0, 100, 150],
]

m = [3, 4, 2, 5]
appr = [-1, 1, -1, -1]

for i in range(len(alt)):
    for j in range(i + 1, len(alt)):
        P = np.zeros((1, 4))[0]
        for k in range(len(m)):
            if(alt[i][k] * appr[k] > alt[j][k] * appr[k]):
                P[k] = m[k]

        print(f'P{i+1}{j+1} = ', end='')
        print(*P, sep=' + ', end=' = ')
        print(sum(P))

        N = np.zeros((1, 4))[0]
        for k in range(len(m)):
            if(alt[i][k] * appr[k] < alt[j][k] * appr[k]):
                N[k] = m[k]

        print(f'N{i+1}{j+1} = ', end='')
        print(*N, sep=' + ', end=' = ')
        print(sum(N))

        print(f'D{i+1}{j+1} = P{i+1}{j+1} / N{i+1}{j+1} = ', end='')
        if sum(N) == 0:
            print('inf - не подходит')
        else:
            D = sum(P) / sum(N)
            print(D, end='')
            if(D > 1):
                print(' - принимаем')
            else:
                print(' - отбрасываем')

        print(f'D{j+1}{i+1} = N{j+1}{i+1} / P{j+1}{i+1} = ', end='')
        if(sum(P) == 0):
            print('inf - не подходит', end='\n\n')
        else:
            D = sum(N) / sum(P)
            print(D, end='')
            if(D > 1):
                print(' - принимаем', end='\n\n')
            else:
                print(' - отбрасываем', end='\n\n')