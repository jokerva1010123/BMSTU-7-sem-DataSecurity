from dataclasses import dataclass

from bitarray import bitarray
from bitarray.util import hex2ba

from encryption.tables import Tables


@dataclass
class Constants:
    START_LEN = 64
    ROUNDS_NUMBER = 16


def get_key_bits(key):
    key_bits = hex2ba(key)

    if len(key_bits) > Constants.START_LEN:
        return key_bits[:Constants.START_LEN]

    zeros = bitarray([0] * (Constants.START_LEN - len(key_bits)))
    return zeros + key_bits


def initial_permutation(key):
    return bitarray([key[bit - 1] for bit in \
                     Tables.KEY_INITIAL_PERMUTATION])


def left_shift(key, offset):
    end = key[:offset]
    key <<= offset

    return key[:-offset] + end


def contraction_permutation(key):
    return bitarray([key[bit - 1] for bit in \
                     Tables.CONTRACTION_PERMUTATION])


def generate_keys(key):
    key = initial_permutation(key)
    c, d = key[:28], key[28:]

    keys = []

    for round in range(Constants.ROUNDS_NUMBER):
        c = left_shift(c, Tables.SHIFT[round])
        d = left_shift(d, Tables.SHIFT[round])

        keys.append(contraction_permutation(c + d))

    return keys
