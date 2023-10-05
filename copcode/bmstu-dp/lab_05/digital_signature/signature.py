from message import (
    success_message, danger_message)

from digital_signature.file_utils import (
    find_file,
    get_signature_path, 
    create_folder,
    get_data_integers, 
    get_file_digest,
    write_data_bytes)
from digital_signature.key_generation import (
    Parameters, generate_keys)
from digital_signature.rsa import (
    decipher_data, encipher_data)


def file_signing(path: str, signature_path: str, private_key: dict) -> None:
    digest = get_file_digest(path)
    cipher_digest = encipher_data(digest, private_key)
    write_data_bytes(signature_path, cipher_digest, 4)


def check_signature(path: str, signature_path: str, public_key: dict) -> bool:
    cipher_digest = get_data_integers(signature_path, 4)
    decipher_digest = decipher_data(cipher_digest, public_key)

    plain_digest = b''

    for number in decipher_digest:
        plain_digest += number.to_bytes(1, 'big')

    digest = get_file_digest(path)
    return digest == plain_digest


def signing() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    public_key, private_key = generate_keys(Parameters.UPPER_BOUND)
    print('\nПолучены ключи:')
    print(f'открытый ключ - {public_key}')
    print(f'закрытый ключ - {private_key}')

    signature_path = get_signature_path(path)
    create_folder('signature/')
    file_signing(path, signature_path, private_key)

    success_message(f'\nПодпись сохранена по пути:{signature_path}')


def verification() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    signature_path = input('\nВведите путь к электронной подписи: ')

    if not find_file(signature_path):
        danger_message('Подпись не найдена.')
        return

    key = input('\nВведите открытый ключ в формате E,N: ').split(',')
    public_key = {'key': int(key[0]), 'module': int(key[1])}

    result = check_signature(path, signature_path, public_key)

    if result:
        success_message('\nДанные подлинны.')
    else:
        danger_message('\nВерификация неуспешна.')
