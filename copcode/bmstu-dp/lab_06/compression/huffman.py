from bitarray import bitarray
from bitarray.util import rindex

from compression.constants import Mode, BitsNumber

from compression.table import (
    get_frequency_table, 
    get_table_bits,
    get_table_bits_len,
    get_table_from_bits)
from compression.tree import Tree


def padding(data: bitarray) -> bitarray:
    data += '1'
    zeros = '0' * (BitsNumber.BYTE - len(data) % BitsNumber.BYTE)

    return data + zeros


def encode_data(data: bytes, codes: dict) -> bitarray:
    encoded_data = bitarray()

    for byte in data:
        encoded_data += bitarray(codes[byte])

    return encoded_data


def decode_data(data: bitarray, codes: dict) -> bytes:
    decoded_data = bytes()

    code = ''

    for bit in data:
        code += str(bit)
        byte = codes.get(code)

        if byte is not None:
            decoded_data += bytes([byte])
            code = ''

    return decoded_data


def compress_data(data: bytes) -> bitarray:
    frequency_table = get_frequency_table(data)
    tree = Tree(None)
    tree.build(frequency_table)

    codes = tree.get_codes(Mode.COMPRESSING)
    compressed_data = encode_data(data, codes)
    table_bits = get_table_bits(frequency_table)

    return padding(table_bits + compressed_data)


def decompress_data(data: bitarray) -> bytes:
    data = data[:rindex(data, 1)]

    table_bits_len = get_table_bits_len(data)
    table = data[:table_bits_len]
    data = data[table_bits_len:]

    frequency_table = get_table_from_bits(table)
    tree = Tree(None)
    tree.build(frequency_table)
    codes = tree.get_codes(Mode.DECOMPRESSING)
    decompressed_data = decode_data(data, codes)

    return decompressed_data
