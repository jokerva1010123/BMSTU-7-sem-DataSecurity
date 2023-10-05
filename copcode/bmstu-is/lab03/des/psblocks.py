from bitarray import bitarray

from des.tables import S3


def add_zeros(bits: bitarray, length: int, right=False) -> bitarray:
    zeros = bitarray([0] * (length - len(bits)))
    return zeros + bits if not right else bits + zeros


def permute(in_array: bitarray, table: list[int]) -> bitarray:
    return bitarray([in_array[i - 1] for i in table])


def substitute(in_array: bitarray, sblock: list[list[int]]) -> bitarray:
    row = int(str(in_array[0]) + str(in_array[-1]), base=2)
    column = int(''.join(str(x) for x in in_array[1:-1].tolist()), base=2)

    return add_zeros(bitarray(bin(sblock[row][column])[2:]), 4)


if __name__ == "__main__":
    a = bitarray('100001')
    print(substitute(a, S3))
