from os.path import isfile, exists
from os import mkdir

from enum import Enum


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


def get_result_path(input_path: str, mode: int) -> str:
    file_name = input_path.split('/')[-1]

    if mode == Mode.ENCIPHERING:
        result_path = 'cipher/'
    elif mode == Mode.DECIPHERING:
        result_path = 'plain/'

    result_path += file_name
    return result_path


def get_data_integers(path: str, length: int) -> list:
    data = []

    with open(path, 'rb') as file:
        while (bytes := file.read(length)):
            data.append(int.from_bytes(bytes, 'big'))

    return data


def write_data_bytes(path: str, data: list, length: int) -> None:
    with open(path, 'wb') as file:
        for number in data:
            file.write(number.to_bytes(length, 'big'))
