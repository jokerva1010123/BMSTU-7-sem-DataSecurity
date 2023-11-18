from bitarray import bitarray
from bitarray.util import hex2ba, ba2hex

from des.tables import IP, IP1
from des.constants import ROUND_NUMBER

from des.psblocks import permute, add_zeros
from des.feistel import feistel
from des.key_generation import generate_keys

def encipher(msg: bitarray, keys: list[bitarray], deciphiring=False):
    if len(msg) != 64:
        return None

    permutation = permute(msg, IP)
    left, right = permutation[:32], permutation[32:]

    if deciphiring:
        left, right = right, left
        keys = reversed(keys)

    for key in keys:
        new_right = left ^ feistel(right, key)
        left = right
        right = new_right

    if deciphiring:
        left, right = right, left

    return permute(left + right, IP1)


def des(msg: bitarray, key: bitarray, deciphiring=False) -> bitarray:
    if len(msg) != 64 or len(key) != 64:
        return None

    keys = generate_keys(key)

    cipher = encipher(msg, keys, deciphiring)

    return cipher


if __name__ == "__main__":
    msg = add_zeros(hex2ba("123456ABCD132536"), 64)
    key = add_zeros(hex2ba("AABB09182736CCDD"), 64)

    cipher = des(msg, key)
    print(ba2hex(msg))
    print(ba2hex(cipher))
    print(ba2hex(des(cipher, key, True)))
