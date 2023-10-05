import sys
import os
import argparse

from termcolor import cprint
from bitarray.util import hex2ba

from des.psblocks import add_zeros
import string_des
import file_des


def print_error(msg: str):
    cprint("ОШИБКА:", "red", end=" ")
    print(msg)


def EncipherString(key: str, deciphering: bool):
    try:
        bitkey = add_zeros(hex2ba(key), 64)
    except ValueError:
        print_error("неверный формат ключа")
        return

    print("Введите строку ([a-z][A-Z][<пробел>][-]):")
    string = input()

    for x in string:
        if x not in string_des.alphabet:
            print_error("недопустимый символ {0}".format(x))
            return

    print("{0} строка:".format("Расшифрованная" if deciphering else
                                     "Зашифрованная"))
    cprint(string_des.string_enciphering(string, bitkey, deciphering), "green")


def EncipherFile(filename: str, key: str,
                 deciphering: bool, outname: str):
    try:
        bitkey = add_zeros(hex2ba(key), 64)
    except ValueError:
        print_error("неверный формат ключа")
        return

    if outname is None:
        outname = "data/result"

    outname += '.' + filename.split('.')[-1]

    os.makedirs(os.path.dirname(outname), exist_ok=True)
    with (open(filename, "rb") as in_file,
          open(outname,  "wb") as out_file):
        pass
        file_des.file_enciphering(in_file, bitkey, deciphering, out_file)

    cprint("Файл {0} успешно {1}!".format(filename,
                             "расшифрован" if deciphering else "зашифрован"),
           "green")
    print("Путь к результату:", outname)


def parse_args():
    parser = argparse.ArgumentParser(
                 description="DES для строк и файлов")
    subparsers = parser.add_subparsers(help="режим работы программы")
    subparsers.required = True

    parser_str = subparsers.add_parser('str', help='шифрование строк')
    parser_str.add_argument(
            '-k', "--key",
            nargs='?',
            help='64-хбитный ключ (16-ричная строка)',
            required=True)
    parser_str.add_argument(
        "-d", "--deciphering", nargs='?',
        const=True, default=False, type=bool,
        help="режим расшифровки"
    )
    parser_str.set_defaults(mode='str')

    parser_file = subparsers.add_parser('file', help='шифрование файла')
    parent_file = parser_file.add_argument('filename', type=str,
                             help='путь к файлу')
    parser_file.add_argument(
            '-k', "--key",
            nargs='?',
            help='64-хбитный ключ (16-ричная строка)',
            required=True)
    parser_file.add_argument(
        "-d", "--deciphering", nargs='?',
        const=True, default=False, type=bool,
        help="режим расшифровки"
    )
    parser_file.add_argument('-o', '--out', nargs='?',
                             help='имя выходного файла')
    parser_file.set_defaults(mode='file')

    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    if args.mode == "str":
        EncipherString(args.key, args.deciphering)

    elif args.mode == "file":
        EncipherFile(args.filename, args.key, args.deciphering, args.out)
