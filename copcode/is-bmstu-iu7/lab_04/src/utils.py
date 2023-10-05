from copy import deepcopy

from numpy import byte


def getTwoBiggestPrimeNumsInRangeN(num: int):
    """
        Используется Решето Эратосфена
        Берутся два самых больших простых числа из получившегося списка
    """
    field = [i for i in range(num + 1)] 
    field[1] = 0 # 1 не простое число

    i = 2

    while (i <= num):
        if (field[i] != 0):
            j = i ** 2

            while (j <= num):
                field[j] = 0
                j += i

        i += 1

    field = list(set(field))
    field.remove(0)

    return field[-2:]


def areRelativelyPrime(num1: int, num2: int):
    """
        Алгоритм Евклида нахождения взаимнопростых чисел
    """
    result = False

    while (num2 != 0):
        tmpNum2 = num2
        num2 = num1 % num2
        num1 = tmpNum2

        if (num1 == 1):
            result = True
            break
    
    return result


def multiplyMatrixes(matA: list, matB: list):
    """
        Перемножение двух матриц
    """

    if (len(matA[0]) != len(matB)):
        return None

    rowNum = len(matA)
    columnNum = len(matB[0])
    
    m = len(matB)

    matRes = [[0 for _ in range(columnNum)] for _ in range(rowNum)]

    for i in range(rowNum):
        for j in range(columnNum):
            matRes[i][j] = sum([matA[i][r] * matB[r][j] for r in range(m)])

    return deepcopy(matRes)


def extendedEuklid(a: int, b: int):
    """
        Расширенный алгоритм Евклида
    """

    eMatrix = \
    [
        [0, 1], 
        [1, 0]
    ]

    bSave = b

    while True:
        r = a % b

        if (r == 0):
            break
        else:
            q = a // b

            matQ = \
            [
                [0, 1],
                [1, -q]
            ]
            eMatrix = multiplyMatrixes(matA=eMatrix, matB=matQ)

            bTemp = b # обычный алгоритм Евклида
            b = a % b
            a = bTemp

    result = eMatrix[1][1]

    # Так как вычисления в кольце, то при отриц прибавить
    if (result < 0):
        result += bSave

    return result


def fastPowerByModule(num: int, power: int, n: int):
    """
        Быстрое возведение в степень по модулю
    """
    r = 1

    while (power > 0):
        if (power & 1):
            r = (r * num) % n
        
        num = (num * num) % n
        power = power >> 1
        
    return r


if __name__ == "__main__":
    pass
