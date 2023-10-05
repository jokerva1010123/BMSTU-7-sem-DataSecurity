INF = -1

def printInfo():
    print("Программа предоставляет интерфейс для поиска расстояния Левенштейна" )


def printAuthor():
    print("АВТОР: Маслова Марина")
    print("ГРУППА: ИУ7-53Б")


def printGreeting():
    print("РАССТОЯНИЕ ЛЕВЕНШТЕЙНА")
    print()
    printInfo();
    print()
    printAuthor();
    print()


def matrixInit(len1, len2):
    matrix = [[i + j if i * j == 0 else INF for j in range(len2 + 1)]
               for i in range(len1 + 1)]

    return matrix


def printDistMatrix(matr, str1, str2):
    print(' ', 'ø', *list(str2), sep = '   ')
    rightColumn = ['ø', *list(str1)]

    for i in range(len(str1) + 1):
        print(rightColumn[i], end = ' ')
        for j in range(len(str2) + 1):
            print('{:3d}'.format(matr[i][j]), end = '\n' 
                                                    if j == len(str2) else ' ')


def recursiveLevenstein(str1, str2):
    len1 = len(str1)
    len2 = len(str2)

    dist = 0
    if len1 == 0:
        dist = len2
    elif len2 == 0:
        dist = len1
    else:
        addDist = 0 if str1[-1] == str2[-1] else 1
        dist = 1 + min(recursiveLevenstein(str1, str2[:-1]),
                       recursiveLevenstein(str1[:-1], str2))
        dist = min(dist, addDist + recursiveLevenstein(str1[:-1], str2[:-1]))

    return dist


def matrixLevenstein(str1, str2):
    len1 = len(str1)
    len2 = len(str2)

    matr = matrixInit(len1, len2)
    dist = 0

    for i in range(1, len1 + 1):
        for j in range(1, len2 + 1):
            addDist = 0 if str1[i - 1] == str2[j - 1] else 1
            dist = 1 + min(matr[i - 1][j], matr[i][j - 1])
            dist = min(dist, addDist + matr[i - 1][j - 1])
            matr[i][j] = dist

    return dist, matr


def recursiveCacheLevenstein(str1, str2, matr):
    len1 = len(str1)
    len2 = len(str2)
    dist = 0

    if matr[len1][len2] == INF:
        addDist = 0 if str1[-1] == str2[-1] else 1
        dist = 1 + min(
                       recursiveCacheLevenstein(str1, str2[:-1], matr),
                       recursiveCacheLevenstein(str1[:-1], str2, matr),
                   )
        dist = min(
                   dist,
                   addDist
                   + recursiveCacheLevenstein(str1[:-1], str2[:-1], matr),
               )
        matr[len1][len2] = dist
    else:
        dist = matr[len1][len2]

    return dist


def cacheLevenstein(str1, str2):
    matr = matrixInit(len(str1), len(str2))

    dist = recursiveCacheLevenstein(str1, str2, matr)

    return dist


def start():
    printGreeting()
    str1 = input("Первая строка: ").lower()
    str2 = input("Вторая строка: ").lower()
    print()
    print("РЕЗУЛЬТАТЫ")
    print("Рекурсивный алгоритм:", recursiveLevenstein(str1, str2))
    matrRes, matr = matrixLevenstein(str1, str2)
    print("Матричный алгоритм:", matrRes)
    print("Рекурсивный алгоритм с кэшем:", cacheLevenstein(str1, str2))
    printDistMatrix(matr, str1, str2)
    print("Спасибо за использование программы!")


if __name__ == "__main__":
    start()
