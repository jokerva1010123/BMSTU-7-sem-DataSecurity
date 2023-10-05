from message import (
    success_message, danger_message)

from compression.constants import Mode

from compression.file_utils import (
    find_file,
    get_data_bytes,
    write_data_bytes,
    get_result_path,
    create_folder,
    get_data_bits,
    write_data_bits)
from compression.huffman import (
    compress_data, decompress_data)


def compress_file() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    data = get_data_bytes(path)
    compressed_data = compress_data(data)

    result_path = get_result_path(path, Mode.COMPRESSING)
    create_folder('compressed/')
    write_data_bits(result_path, compressed_data)

    success_message(f'\nПуть к сжатому файлу: {result_path}')


def decompress_file() -> None:
    path = input('\nВведите путь к файлу: ')

    if not find_file(path):
        danger_message('Файл не найден.')
        return

    data = get_data_bits(path)
    decompressed_data = decompress_data(data)

    result_path = get_result_path(path, Mode.DECOMPRESSING)
    create_folder('decompressed/')
    write_data_bytes(result_path, decompressed_data)

    success_message(f'\nПуть к восстановленному файлу: {result_path}')
