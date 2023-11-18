from bitarray import bitarray
from bitarray.util import hex2ba, ba2hex

from des.psblocks import permute, substitute, add_zeros
from des.tables import E, S, P


def feistel(msg: bitarray, key: bitarray) -> bitarray:
    if len(msg) != 32 or len(key) != 48:
        return None

    perm_msg = permute(msg, E) ^ key
    result = bitarray()

    for i in range(len(S)):
        result += substitute(perm_msg[i * 6:(i + 1) * 6], S[i])

    result = permute(result, P)

    return result


if __name__ == "__main__":
    print(ba2hex(add_zeros(hex2ba('14A7D678'), 32) ^
                 feistel(add_zeros(hex2ba('18CA18AD'), 32),
                         hex2ba('194CD072DE8C'))))
