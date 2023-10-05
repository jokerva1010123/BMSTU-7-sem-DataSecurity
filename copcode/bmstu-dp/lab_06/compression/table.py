from bitarray import bitarray
from bitarray.util import ba2int

from compression.constants import BitsNumber


def get_table_bits_len(data) -> int:
    items_number = ba2int(data[:BitsNumber.FREQUENCY])
    items_bits_len = BitsNumber.BYTE + BitsNumber.FREQUENCY

    return BitsNumber.FREQUENCY + items_number * items_bits_len


def get_frequency_table(data: bytes) -> dict:
    table = {}

    for byte in data:
        if table.get(byte) is None:
            table[byte] = 0
        table[byte] += 1

    return table


def get_table_bits(table: dict) -> bitarray:
    bits = bitarray()

    items_number = len(table)
    bits += f'{items_number:016b}'

    for item in table.items():
        byte_bits = f'{item[0]:08b}'
        frequency_bits = f'{item[1]:016b}'
        bits += byte_bits + frequency_bits

    return bits


def get_table_from_bits(bits: bitarray) -> dict:
    table = {}

    items_number = ba2int(bits[:BitsNumber.FREQUENCY])
    bits = bits[BitsNumber.FREQUENCY:]

    for i in range(items_number):
        byte_start = i * (BitsNumber.BYTE + BitsNumber.FREQUENCY)
        byte_end = byte_start + BitsNumber.BYTE
        byte = ba2int(bits[byte_start:byte_end])

        frequency_end = byte_end + BitsNumber.FREQUENCY
        frequency = ba2int(bits[byte_end:frequency_end])

        table[byte] = frequency

    return table
