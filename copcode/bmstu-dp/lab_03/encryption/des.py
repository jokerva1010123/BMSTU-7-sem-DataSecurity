from dataclasses import dataclass

from bitarray import bitarray

from encryption.feistel import feistel_cipher
from encryption.file_utils import Mode
from encryption.tables import Tables


@dataclass
class Constants:
    BLOCK_LEN = 64
    ROUNDS_NUMBER = 16


def padding(data):
    data.append(1)
    zeros = [0] * (Constants.BLOCK_LEN - len(data) % Constants.BLOCK_LEN)
    data.extend(zeros)

    return data


def initial_permutation(input_block):
    return bitarray([input_block[bit - 1] for bit in \
                     Tables.INITIAL_PERMUTATION])


def final_permutation(input_block):
    result_block = bitarray([0] * Constants.BLOCK_LEN)
    
    for index, bit in enumerate(Tables.INITIAL_PERMUTATION):
        result_block[bit - 1] = input_block[index]

    return result_block


def encipher_block(block, keys, mode):
    block = initial_permutation(block)
    left, right = block[:32], block[32:]

    if mode == Mode.DECIPHERING:
        left, right = right, left

    for round in range(Constants.ROUNDS_NUMBER):
        left, right = right, left ^ feistel_cipher(right, keys[round])

    if mode == Mode.DECIPHERING:
        left, right = right, left

    return final_permutation(left + right)
