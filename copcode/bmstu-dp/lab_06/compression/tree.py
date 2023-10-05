from compression.constants import Mode
from compression.node import Node


class Tree:
    def __init__(self, root: Node) -> None:
        self.root = root

    def build(self, frequency_table: dict) -> None:
        nodes = []

        for item in frequency_table.items():
            nodes.append(Node(item[0], item[1], None, None))

        while len(nodes) != 1:
            nodes.sort(key= lambda node: node.frequency)

            first_node = nodes[0]
            second_node = nodes[1]
            nodes = nodes[2:]

            node = Node(None, first_node.frequency + \
                        second_node.frequency,
                        first_node, second_node)

            nodes.append(node)

        self.root = nodes[0]

    def encode(self, node, code, codes_table, mode) -> None:
        if node.left_child is None and \
           node.right_child is None:

           if mode == Mode.COMPRESSING:
               codes_table[node.value] = code
           elif mode == Mode.DECOMPRESSING:
               codes_table[code] = node.value

        if node.left_child is not None:
            self.encode(node.left_child, code + '0', codes_table, mode)

        if  node.right_child is not None:
            self.encode(node.right_child, code + '1', codes_table, mode)

    def get_codes(self, mode) -> dict:
        codes_table = {}

        if self.root is not None:
            self.encode(self.root, '', codes_table, mode)

        return codes_table
