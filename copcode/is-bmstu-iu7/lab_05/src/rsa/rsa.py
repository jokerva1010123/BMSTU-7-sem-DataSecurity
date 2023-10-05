from rsa.utils import *

KEY = 0
SIZE = 1
PRIME_SIZE = 10000


def getKeyD(phi: int) -> int:
    for num in range(phi // 100, phi // 10000, -1):
        if (areRelativelyPrime(phi, num)):
            keyD = num
            break

    return keyD


def getKeyE(phi: int, keyD: int) -> int:
    # Необходимо решить следующее соотношение
    # (keyE * keyD) mod (phi) = 1 =>
    # (keyE * keyD) = 1 + k * phi =>
    # (keyD) * keyE - (phi) * k = 1 <=> 
    # ax + by = НОД(a, b) = 1, где a = keyD, b = phi

    return extendedEuklid(keyD, phi)


def getKeys():
    # 1. Простые p, q
    p, q = getTwoBiggestPrimeNumsInRangeN(PRIME_SIZE)

    # 2. n
    n = p * q

    # 3. Функция Эйлера
    phi = (p - 1) * (q - 1)

    # 4. Ключ D поиском взаимно простого числа с phi
    keyD = [getKeyD(phi), n]

    # 5. Ключ E с помощью расширенного алогритма Евклида
    keyE = [getKeyE(phi, keyD[KEY]), n]

    return keyD, keyE


def rsaCypher(element: int, key: list[int]):
    return fastPowerByModule(element, key[KEY], key[SIZE])
