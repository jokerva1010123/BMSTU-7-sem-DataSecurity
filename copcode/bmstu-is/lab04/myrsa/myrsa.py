from typing import Tuple
from myrsa.utils import ModularPow

def CryptBase(value: int, key: Tuple[int, int]) -> int:
    return ModularPow(value, key[0], key[1])


def Encrypt(value: int, key: Tuple[int, int]) -> int:
    return CryptBase(value, key)


def Decrypt(value: int, key: Tuple[int, int]) -> int:
    return CryptBase(value, key)


if __name__ == "__main__":
    print(Encrypt(8,  (13, 33)))
    print(Encrypt(27, (13, 33)))
    print(Encrypt(5,  (13, 33)))

    print(Decrypt(17, (17, 33)))
    print(Decrypt(15, (17, 33)))
    print(Decrypt(26, (17, 33)))
