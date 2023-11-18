from typing import Tuple


def gcd(a: int, b: int) -> int:
    while b != 0:
        a, b = b, a % b

    return a


def ExtendedEuclideanAlgorithm(a: int, b: int) -> Tuple[int, int, int]:
    swap = False
    if a < b:
        a, b = b, a
        swap = True

    first = (a, 1, 0)
    second = (b, 0, 1)

    while second[0] != 0:
        quotient = first[0] // second[0]
        first, second = second, (first[0] % second[0],
                                 first[1] - second[1] * quotient,
                                 first[2] - second[2] * quotient)

    if swap:
        first = (first[0], first[2], first[1])

    return first


def ModularPow(base: int, exponent: int, module: int) -> int:
    if module == 1:
        return 0

    result = 1
    base %= module

    while exponent != 0:
        if exponent % 2:
            result = result * base % module

        exponent //= 2
        base = base ** 2 % module

    return result


if __name__ == "__main__":
    print(gcd(2, 4))
    print(gcd(10, 5))
    print(gcd(20, 11))
    print(gcd(11, 0))

    print(ExtendedEuclideanAlgorithm(2, 4))
    print(ExtendedEuclideanAlgorithm(10, 5))
    print(ExtendedEuclideanAlgorithm(20, 11))
    print(ExtendedEuclideanAlgorithm(11, 0))

    print(ModularPow(4, 13, 497))
    print(ModularPow(2, 50, 100))
