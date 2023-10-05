from menu import Menu
from message import danger_message

from encryption.file_encryption import (
    encipher_file, decipher_file)


def main() -> None:
    run = True

    while run:
        choice = int(input(Menu.TEXT))

        if choice == Menu.ENCIPHERING:
            encipher_file()
        elif choice == Menu.DECIPHERING:
            decipher_file()
        elif choice == Menu.EXIT:
            run = False
        else:
            danger_message('Выбран неверный пункт меню.')


if __name__ == '__main__':
    main()
