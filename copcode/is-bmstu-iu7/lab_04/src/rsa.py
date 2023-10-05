from utils import *
from defines import *
import config as cfg


def getKeyE(phi: int) -> int:
    """
        Сгенерировать публичный ключ E
        
        По алгоритму Евклида поиска взаимнопростых чисел
    """
    for num in range(phi // 100, phi // 10000, -1):
        if (areRelativelyPrime(phi, num)):
            keyE = num
            break

    return keyE


def getKeyD(phi: int, keyE: int) -> int:
    """
        Сгенерировать секретный ключ D
        
        По расширенному алгоритму Евклида
    """
    # Необходимо решить следующее соотношение
    # (keyE * keyD) mod (phi) = 1 =>
    # (keyE * keyD) = 1 + k * phi =>
    # (keyE) * keyD - (phi) * k = 1 <=> 
    # ax + by = НОД(a, b) = 1, где a = keyE, b = phi
    
    # keyE * keyD + phi * k = НОД(keyE, phi) = 1

    return extendedEuklid(keyE, phi)


def getKeys():
    """
        Сгенерировать ключи E и D
    """
    # 1. Простые p, q
    p, q = getTwoBiggestPrimeNumsInRangeN(cfg.primeSize)

    # 2. n
    n = p * q

    # 3. Функция Эйлера
    phi = (p - 1) * (q - 1)

    # 4. Ключ E поиском взаимно простого числа с phi
    keyE = [getKeyE(phi), n]

    # 5. Ключ D с помощью расширенного алогритма Евклида
    keyD = [getKeyD(phi, keyE[KEY]), n]

    return keyD, keyE


def rsaCypher(element: int, key: list[int]):
    """
        Шифрование алгоритмом RSA
    """
    return fastPowerByModule(element, key[KEY], key[SIZE])


if __name__ == "__main__":
    # 1. Простые p, q
    p, q = getTwoBiggestPrimeNumsInRangeN(10000)

    print("prime nums =", p, q)
    n = p * q
    print("n =", n)
    phi = (p - 1) * (q - 1)
    print("phi =", phi)

    # 2. Ключ D поиском взаимно простого числа с phi
    keyD = getKeyD(phi)

    # 3. Ключ E с помощью расширенного алогритма Евклида
    keyE = getKeyE(phi, keyD)

    print("keyE = {0}; keyD = {1}".format(keyE, keyD))
    print("CHECK: (keyE * keyD) mod(phi) = {}".format((keyE * keyD) % ((p - 1) * (q - 1))))

    # 4. Шифровка
    toEncrypt = 15
    encrypted = rsaCypher(toEncrypt, keyE) 
    decrypted = rsaCypher(encrypted, keyD)

    print("t = {0}; e = {1}; d = {2}".format(toEncrypt, encrypted, decrypted))

