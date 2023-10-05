from bitarray import bitarray
from bitarray.util import ba2int

from huffman.table import FrequencyTable
from huffman.tree import HuffmanTree

class HuffmanCoder:

    def __init__(self):
        pass

    def Compress(self, what: bytes) -> bitarray:
        freqTable = FrequencyTable()
        freqTable.Build(what)
        codesTable = HuffmanTree(freqTable).GetHuffmanCodeTable()
        compressed = self.compressByCodesTable(what, codesTable)

        result = freqTable.ToBitarray() + compressed

        return result


    def compressByCodesTable(self, what: bytes, codesTable: dict):
        result = bitarray()
        for byte in what:
            result += codesTable[byte]

        return result


    def Decompress(self, what: bitarray) -> bytes:
        bitTable, data = self.extract(what)

        freqTable = FrequencyTable()
        freqTable.FromBitarray(bitTable)
        codesTable = HuffmanTree(freqTable).GetHuffmanCodeTable(True)

        decompressed = self.decompressByCodesTable(data, codesTable)

        return decompressed


    def extract(self, what: bitarray) -> tuple[bitarray, bitarray]:
        symbolsNum = ba2int(what[:8])
        tableLen = 32 + 24 * symbolsNum
        # tableLen = 8 * (1 + 3  * (symbolsNum + 1))

        return what[:tableLen], what[tableLen:]


    def decompressByCodesTable(self, what: bitarray, codesTable: dict):
        result = bytes()
        curCode = bitarray()
        for bit in what:
            curCode += bitarray(str(bit))
            trueByte = codesTable.get(curCode.to01())

            if trueByte is not None :
                curCode = bitarray()
                result += bytes([trueByte])

        return result

