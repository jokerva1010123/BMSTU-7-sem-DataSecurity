from os.path import isfile, exists
from os import mkdir

from enum import Enum
from bitarray import bitarray


class Mode(Enum):
    ENCIPHERING = 1
    DECIPHERING = 2


def find_file(path: str) -> bool:
    if not exists(path) or not isfile(path):
        return False
    return True


def create_folder(path: str) -> None:
    if not exists(path):
        mkdir(path)


def get_result_path(file_path: str, mode: int) -> str:
    file_name = file_path.split('/')[-1]

    if mode == Mode.ENCIPHERING:
        result_path = 'cipher/'
    elif mode == Mode.DECIPHERING:
        result_path = 'plain/'

    result_path += file_name
    return result_path


def get_data_bits(path:str) -> bitarray:
    data = bitarray()

    with open(path, 'rb') as file:
        data.fromfile(file)

    return data


def write_data_bits(path: str, data: bitarray) -> None:
    with open(path, 'wb') as file:
        data.tofile(file)
