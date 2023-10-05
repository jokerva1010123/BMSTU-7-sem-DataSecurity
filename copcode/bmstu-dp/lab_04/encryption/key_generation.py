from random import sample, randint
from dataclasses import dataclass


@dataclass
class Parameters:
    UPPER_BOUND = 50000
    PERCENT_NOT_USED = 24
    MIN_INDEX = 2


def eratosthenes_sieve(bound: int) -> list:
    numbers = [False, False] + [True] * (bound - 1)

    for i in range(2, bound + 1):
        if numbers[i]:
            for j in range(i * i, bound + 1, i):
                numbers[j] = False

    return [i for i, number in enumerate(numbers) if number]


def euler_function(p: int, q: int) -> int:
    return (p - 1) * (q - 1)


def get_gcd(a: int, b: int) -> int:
    while b:
        a, b = b, a % b

    return a


def get_coprime_number(number: int) -> int:
    coprime_number = randint(2, number - 1)

    while get_gcd(number, coprime_number) != 1:
        coprime_number = randint(2, number - 1)

    return coprime_number


def extended_euclidean_algorithm(a: int, b: int) -> list:
    x1, y1 = 1, 0
    x2, y2 = 0, 1

    while b:
        quotient = a // b
        a, b = b, a % b

        x1, x2 = x2, x1 - x2 * quotient
        y1, y2 = y2, y1 - y2 * quotient

    return [a, x1, y1]


def generate_keys(bound: int) -> list:
    prime_numbers = eratosthenes_sieve(bound)
    lower_index = len(prime_numbers) * Parameters.PERCENT_NOT_USED \
                  // 100 + 1
    lower_index = max(Parameters.MIN_INDEX, lower_index)
    prime_numbers = prime_numbers[lower_index:]

    p, q = sample(prime_numbers, 2)
    module = p * q
    fi = euler_function(p, q)

    e = get_coprime_number(fi)
    d = extended_euclidean_algorithm(fi, e)[2]
    d %= fi

    public_key = {'key': e, 'module': module}
    private_key = {'key': d, 'module': module}

    return [public_key, private_key]
