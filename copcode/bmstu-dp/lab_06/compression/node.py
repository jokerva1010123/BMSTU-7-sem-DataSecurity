class Node:
    def __init__(self, value, frequency, left_child, right_child):
        self.value = value
        self.frequency = frequency
        self.left_child = left_child
        self.right_child = right_child

    def print_node(self) -> None:
        if self.left_child is not None:
            self.left_child.print_node()

        print(self.frequency)

        if self.right_child is not None:
            self.right_child.print_node()
