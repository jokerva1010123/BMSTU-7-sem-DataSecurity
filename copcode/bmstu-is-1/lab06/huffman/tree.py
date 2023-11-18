from bitarray import bitarray

from huffman.table import FrequencyTable

class HuffmanNode:

    def __init__(self
                 , symbol: int
                 , frequency: int
                 , left=None
                 , right=None
                 ):
        self.symbol = symbol
        self.frequency = frequency
        self.leftChild = left
        self.rightChild = right

    def __str__(self):
        return (f"[({self.symbol} : {self.frequency})"
               + f"\nleft={self.leftChild}"
               + f"\nright={self.rightChild}]")

    def __lt__(self, other):
        return self.frequency < other.frequency

    def __le__(self, other):
        return self.frequency <= other.frequency

    def __eq__(self, other):
        return self.frequency == other.frequency

    def __ne__(self, other):
        return self.frequency != other.frequency

    def __gt__(self, other):
        return self.frequency > other.frequency

    def __ge__(self, other):
        return self.frequency >= other.frequency


class HuffmanTree:

    def __init__(self, table: FrequencyTable):
        self.root = None
        self.build(table)


    def build(self, table: FrequencyTable):
        nodes = [HuffmanNode(*item) for item in table]

        while len(nodes) != 1:
            firstMin = min(nodes)
            nodes.remove(firstMin)

            secondMin = min(nodes)
            nodes.remove(secondMin)

            newNode = HuffmanNode(None
                                  , firstMin.frequency + secondMin.frequency
                                  , firstMin
                                  , secondMin
                                  )
            nodes.append(newNode)

        self.root = nodes[0]


    def recurciveGetTable(self
                          , curRoot: HuffmanNode
                          , curCode: bitarray
                          , swap: bool):
        if curRoot.symbol is not None:
            return ({curCode.to01() : curRoot.symbol}
                    if swap else {curRoot.symbol : curCode})

        return (self.recurciveGetTable(curRoot.leftChild, curCode + '0', swap)
              | self.recurciveGetTable(curRoot.rightChild, curCode + '1', swap))


    def GetHuffmanCodeTable(self, swap=False):
        return self.recurciveGetTable(self.root, bitarray(), swap)
