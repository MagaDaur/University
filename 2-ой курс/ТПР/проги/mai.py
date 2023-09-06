from data import matrix


def synthesis_of_priorities(matrix):
    sum_V = 0

    for line in matrix:
        V = round(pow(line[0] * line[1] * line[2] * line[3] * line[4], 0.2), 3)
        line.append(V)
        sum_V = round(sum_V + V, 3)

    for line in matrix:
        W = round(line[5] / sum_V, 3)
        line.append(W)

    return matrix


def synthesis_of_local_priorities(matrix, id):
    S1 = 0
    S2 = 0
    S3 = 0
    S4 = 0
    S5 = 0

    for line in matrix:
        S1 = S1 + line[0]
        S2 = S2 + line[1]
        S3 = S3 + line[2]
        S4 = S4 + line[3]
        S5 = S5 + line[4]

    P1 = S1 * matrix[0][6]
    P2 = S2 * matrix[1][6]
    P3 = S3 * matrix[2][6]
    P4 = S4 * matrix[3][6]
    P5 = S5 * matrix[4][6]

    lamda = P1 + P2 + P3 + P4 + P5
    index_sogl = (lamda - 5) / 4
    res_sogl = index_sogl / 1.12
    print("Лямда =", lamda)


    if res_sogl <= 0.10:
        print(f'\n> Матрица {id}  согласована!')
        print("\n------------------------------------------------------------")
    else:
        print('Матрица не согласована', res_sogl)
        print("\n------------------------------------------------------------")


def print_matrix(matrix, id):

    print()
    print(id, end="\t\t")
    for i in range(1, 6):
        print(str(i), end="\t\t")
    print('V\t\tW')

    for j in range(0, 5):
        for i in range(0, 7, 1):
            if i == 0:
                print(str(j + 1), end="\t\t")

            line = round(float(matrix[j][i]), 1)
            print(line, end='\t    ')
        print()


if __name__ == '__main__':
    new_purpose = matrix.purpose
    main_matrix = synthesis_of_priorities(new_purpose)
    print_matrix(main_matrix, 'Pur')
    synthesis_of_local_priorities(main_matrix, 'Цель')

    new_durable = matrix.durable
    K1 = synthesis_of_priorities(new_durable)
    print_matrix(K1, 'K1')
    synthesis_of_local_priorities(K1, 'Проходной балл')

    new_controllability = matrix.controllability
    K2 = synthesis_of_priorities(new_controllability)
    print_matrix(K2, 'K2')
    synthesis_of_local_priorities(K2, 'Кол-во бюджетных мест')

    new_stability = matrix.stability
    K3 = synthesis_of_priorities(new_stability)
    print_matrix(K3, 'K3')
    synthesis_of_local_priorities(K3, 'Цена')

    new_count_layer = matrix.count_layers
    K4 = synthesis_of_priorities(new_count_layer)
    print_matrix(K4, 'K4')
    synthesis_of_local_priorities(K4, 'Рейтинг')

    new_count_bearings = matrix.count_bearings
    K5 = synthesis_of_priorities(new_count_bearings)
    print_matrix(K5, 'K5')
    synthesis_of_local_priorities(K5, 'Время поездки')



purpose = [[1, 3, 3, 5, 5],
           [0.33, 1, 3, 3, 3],
           [0.33, 0.33, 1, 3, 5],
           [0.2, 0.33, 0.33, 1, 3],
           [0.2, 0.33, 0.2, 0.33, 1]]

durable = [[1, 0.33, 0.33, 0.11, 0.11],
           [3, 1, 0.33, 0.11, 0.11],
           [3, 3, 1, 0.14, 0.14],
           [9, 9, 7, 1, 1],
           [9, 9, 7, 1, 1]]

controllability = [[1, 0.2, 0.14, 0.11, 0.11],
                   [5, 1, 0.14, 0.11, 0.11],
                   [7, 7, 1, 0.33, 0.33],
                   [9, 9, 3, 1, 1],
                   [9, 9, 3, 1, 1]]

stability = [[1, 0.2, 0.14, 0.11, 0.11],
             [5, 1, 0.14, 0.11, 0.11],
             [7, 7, 1, 0.33, 0.33],
             [9, 9, 3, 1, 1],
             [9, 9, 3, 1, 1]]

count_layers = [[1, 0.33, 0.2, 0.11, 0.11],
                [3, 1, 0.14, 0.11, 0.14],
                [5, 7, 1, 0.2, 0.33],
                [9, 9, 5, 1, 3],
                [7, 7, 3, 0.2, 1]]

count_bearings = [[1, 3, 5, 7, 9],
                  [0.33, 1, 3, 5, 9],
                  [0.2, 0.33, 1, 3, 5],
                  [0.14, 0.2, 0.33, 1, 5],
                  [0.11, 0.11, 0.2, 0.2, 1]]

