from bitarray import bitarray
from bitarray.util import ba2int
from collections import Counter


class FrequencyTable:

    def __init__(self):
        self.table = None

    def __iter__(self):
        return iter(self.table.items())

    def Build(self, what: bytes):
        # self.table = Counter(what)
        self.table = Counter()
        for byte in what:
            self.table[byte] += 1

    def FromBitarray(self, what: bitarray):
        self.table = Counter()
        symbolsNum = ba2int(what[:8]) + 1

        for i in range(symbolsNum):
            byteBegin = 8 + 24 * i
            byteEnd = byteBegin + 8
            freqEnd = byteEnd + 16
            byte = ba2int(what[byteBegin:byteEnd])
            frequency = ba2int(what[byteEnd:freqEnd])
            self.table[byte] = frequency


    def ToBitarray(self) -> bitarray:
        result = bitarray("{0:08b}".format(len(self.table) - 1))
        for item in self.table.items():
            itemCode = f"{'{0:08b}'.format(item[0])}{'{0:016b}'.format(item[1])}"
            result += itemCode

        return result

