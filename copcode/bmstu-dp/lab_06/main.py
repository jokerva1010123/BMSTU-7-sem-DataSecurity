from menu import Menu
from message import danger_message

from compression.file_compression import (
    compress_file, decompress_file)


def main() -> None:
    run = True

    while run:
        choice = int(input(Menu.TEXT))

        if choice == Menu.COMPRESSING:
            compress_file()
        elif choice == Menu.DECOMPRESSING:
            decompress_file()
        elif choice == Menu.EXIT:
            run = False
        else:
            danger_message('Выбран неверный пункт меню.')


if __name__ == '__main__':
    main()
