from bitarray import bitarray
from bitarray.util import rindex

from message import (
    success_message, danger_message)

from encryption.file_utils import (
    Mode,
    find_file,
    get_result_path,
    create_folder,
    get_data_bits,
    write_data_bits)
from encryption.key_generation import (
    get_key_bits, generate_keys)
from encryption.des import (
    Constants, 
    padding,
    encipher_block)


def des_enciphering(file, mode=Mode.ENCIPHERING):
    key = input('Введите ключ: ')
    key_bits = get_key_bits(key)
    keys = generate_keys(key_bits)

    if mode == Mode.DECIPHERING:
        keys.reverse()

    data_bits = get_data_bits(file)

    if mode == Mode.ENCIPHERING:
        data_bits = padding(data_bits)

    result = bitarray()

    for i in range(0, len(data_bits), Constants.BLOCK_LEN):
        result.extend(encipher_block(data_bits[i:i + Constants.BLOCK_LEN],
                                     keys, mode))

    if mode == Mode.DECIPHERING:
        result = result[:rindex(result, 1)]

    return result


def encipher_file() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    cipher_data = des_enciphering(path)

    result_path = get_result_path(path, Mode.ENCIPHERING)
    create_folder('cipher/')
    write_data_bits(result_path, cipher_data)

    success_message(f'\nПуть к зашифрованному файлу: {result_path}')


def decipher_file() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    plain_data = des_enciphering(path, Mode.DECIPHERING)

    result_path = get_result_path(path, Mode.DECIPHERING)
    create_folder('plain/')
    write_data_bits(result_path, plain_data)

    success_message(f'\nПуть к расшифрованному файлу: {result_path}')
