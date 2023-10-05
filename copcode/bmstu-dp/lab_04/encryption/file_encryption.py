from message import (
    success_message, danger_message)

from encryption.file_utils import (
    Mode,
    find_file,
    get_data_integers,
    get_result_path,
    create_folder,
    write_data_bytes)
from encryption.rsa import (
    encipher_data, decipher_data)
from encryption.key_generation import (
    Parameters, generate_keys)


def encipher_file() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    public_key, private_key = generate_keys(Parameters.UPPER_BOUND)
    print('\nПолучены ключи:')
    print(f'открытый ключ - {public_key}')
    print(f'закрытый ключ - {private_key}')

    data = get_data_integers(path, 1)
    cipher_data = encipher_data(data, public_key)

    result_path = get_result_path(path, Mode.ENCIPHERING)
    create_folder('cipher/')
    write_data_bytes(result_path, cipher_data, 4)

    success_message(f'\nПуть к зашифрованному файлу: {result_path}')


def decipher_file() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    key = input('\nВведите закрытый ключ в формате D,N: ').split(',')
    private_key = {'key': int(key[0]), 'module': int(key[1])}

    data = get_data_integers(path, 4)
    create_folder('plain/')
    plain_data = decipher_data(data, private_key)

    result_path = get_result_path(path, Mode.DECIPHERING)
    write_data_bytes(result_path, plain_data, 1)

    success_message(f'\nПуть к расшифрованному файлу: {result_path}')
