from typing import BinaryIO
from bitarray import bitarray
from bitarray.util import rindex

from des.psblocks import add_zeros
from des.des import des

def file_enciphering(in_file: BinaryIO, bitkey: bitarray,
                     deciphering: bool, out_file: BinaryIO):
    msg = bitarray()
    while (byte := in_file.read(1)):
        tmp = bitarray()
        tmp.frombytes(byte)
        msg += tmp

    if not deciphering:
        msg += bitarray('1')

    if len(msg) % 64:
        msg = add_zeros(msg, len(msg) // 64 * 64 + 64, True)

    result = bitarray()
    for i in range(0, len(msg), 64):
        result += des(msg[i:i + 64], bitkey, deciphering)

    if deciphering:
        result = result[:rindex(result, 1)]

    out_file.write(result.tobytes())
