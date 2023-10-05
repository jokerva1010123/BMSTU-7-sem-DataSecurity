from menu import Menu
from message import danger_message

from digital_signature.signature import (
    signing, verification)


def main() -> None:
    run = True

    while run:
        choice = int(input(Menu.TEXT))

        if choice == Menu.SIGNING:
            signing()
        elif choice == Menu.VERIFICATION:
            verification()
        elif choice == Menu.EXIT:
            run = False
        else:
            danger_message('Выбран неверный пункт меню.')


if __name__ == '__main__':
    main()
