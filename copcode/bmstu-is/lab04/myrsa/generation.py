import logging

from typing import Tuple, Set
import random

from myrsa.utils import gcd, ExtendedEuclideanAlgorithm

Key = Tuple[int, int]

def EratosthenesSieve(upper: int) -> Set[int]:
    logging.info("EratosthenesSieve")
    table = [x for x in range(upper + 1)]
    table[1] = 0

    for num in table:
        if num != 0:
            for j in range(num * num, upper + 1, num):
                table[j] = 0

    return [x for x in table if x != 0]


def GetCoprime(number: int) -> int:
    logging.info("GetCoprime")
    candidate = random.randint(2, number - 1)
    while gcd(candidate, number) != 1:
        candidate = random.randint(2, number - 1)

    return candidate


def GetModularInverse(number: int, module: int) -> int:
    logging.info("GetModularInverse")
    e = ExtendedEuclideanAlgorithm(number, module)[1]
    e = e + module if e < 0 else e
    return e


def GenerateKeys(upper: int) -> Tuple[Key, Key]:
    logging.info("GenerateKeys")
    primeNumsTable = list(EratosthenesSieve(upper))
    logging.debug(f"primeTableLen {len(primeNumsTable)}")
    logging.debug(f"primeTable {primeNumsTable}")
    logging.debug(f"len part of table {max(len(primeNumsTable) // 10 + 1, 2)}")
    greatestPrimes = primeNumsTable[-max(len(primeNumsTable) // 10 + 1, 2):]
    logging.debug(f"part of table {greatestPrimes[0]}, {greatestPrimes[1]}")

    p, q = random.sample(greatestPrimes, 2)
    logging.debug(f"p={p}, q={q}")
    N = p * q
    logging.debug(f"N={N}")
    EulerFunc = (p - 1) * (q - 1)
    logging.debug(f"f={EulerFunc}")

    d = GetCoprime(EulerFunc)
    e = GetModularInverse(d, EulerFunc)
    logging.debug(f"d={d}, e={e}")

    return (d, N), (e, N)
