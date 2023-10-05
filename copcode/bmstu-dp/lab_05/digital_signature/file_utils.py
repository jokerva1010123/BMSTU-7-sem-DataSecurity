from os.path import isfile, exists
from os import mkdir

from hashlib import sha256


def find_file(path: str) -> bool:
    if not exists(path) or not isfile(path):
        return False
    return True


def create_folder(path: str) -> None:
    if not exists(path):
        mkdir(path)


def get_signature_path(input_path: str) -> str:
    file_name = input_path.split('/')[-1]
    dot_index = file_name.rindex('.')

    return 'signature/' + file_name[:dot_index]


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


def get_file_digest(path: str) -> bytes:
    with open(path, 'rb') as file:
        data = file.read()
        digest = sha256(data).digest()

    return digest
