from os.path import isfile, exists
from os import mkdir

from bitarray import bitarray

from compression.constants import Mode


def find_file(path: str) -> bool:
    if not exists(path) or not isfile(path):
        return False
    return True


def create_folder(path: str) -> None:
    if not exists(path):
        mkdir(path)


def get_result_path(input_path: str, mode: int) -> str:
    file_name = input_path.split('/')[-1]

    if mode == Mode.COMPRESSING:
        result_path = 'compressed/'
    elif mode == Mode.DECOMPRESSING:
        result_path = 'decompressed/'

    result_path += file_name
    return result_path


def get_data_bytes(path: str) -> bytes:
    with open(path, 'rb') as file:
        data = file.read()

    return data


def write_data_bytes(path: str, data: bytes) -> None:
    with open(path, 'wb') as file:
        file.write(data)


def get_data_bits(path: str) -> bitarray:
    data = bitarray()

    with open(path, 'rb') as file:
        data.fromfile(file)
    
    return data


def write_data_bits(path:str, data: bitarray) -> None:
        with open(path, "wb") as file:
            data.tofile(file)
