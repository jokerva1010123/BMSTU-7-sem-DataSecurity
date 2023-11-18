from bitarray import bitarray
from bitarray.util import hex2ba, ba2hex

from des.constants import ROUND_NUMBER
from des.tables import B, SI, CP

from des.psblocks import permute, add_zeros


def bitarray_circular_shift(bits: bitarray, step: int) -> bitarray:
    return add_zeros(bits[:step], len(bits)) | (bits << step)


def generate_keys(key: bitarray) -> list[bitarray]:
    if len(key) != 64:
        return []

    result = permute(key, B)

    ci, di = result[:28], result[28:]

    keys = []

    for i in range(ROUND_NUMBER):
        ci = bitarray_circular_shift(ci, SI[i])
        di = bitarray_circular_shift(di, SI[i])
        keys.append(permute(ci+di, CP))

    return keys


if __name__ == "__main__":
    generate_keys(hex2ba('AABB09182736CCDD'))

