from enum import Enum
from dataclasses import dataclass


class Mode(Enum):
    COMPRESSING = 1
    DECOMPRESSING = 2


@dataclass
class BitsNumber:
    BYTE = 8
    FREQUENCY = 16
