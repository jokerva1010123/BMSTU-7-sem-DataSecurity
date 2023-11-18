import sys
import argparse

from enigma.string_enigma import StringEnigma
from enigma.text_enigma import TextEnigma
from enigma.bin_enigma import BinEnigma
from enigma.enigma import Enigma
import enigma.config_parser as ConfPars

from config import alphabet, bin_alphabet


def EncipherString(conf):
    enigma = Enigma(alphabet)

    if conf is not None:
        enigmaConfig = ConfPars.LoadConfig(conf)
        enigma.SetConfig(*enigmaConfig)
        print("Настройки Энигмы успешно загружены из файла", conf)

    strEnigma = StringEnigma(enigma)

    inString = input("Введите сообщение для шифрования:\n")
    encodedString = strEnigma.Encipher(inString)
    print("Зашифрованное сообщение:", encodedString, sep="\n")

    if conf is None:
        ConfPars.SaveConfig(strEnigma.GetEnigmaConfig(), "config/str")
        print("Настройки Энигмы успешно сохранены в файл", "config/str")


def EncipherText(filename, conf, outfilename):
    enigma = Enigma(alphabet)

    if conf is not None:
        enigmaConfig = ConfPars.LoadConfig(conf)
        enigma.SetConfig(*enigmaConfig)
        print("Настройки Энигмы успешно загружены из файла", conf)

    textEnigma = TextEnigma(enigma)

    try:
        textEnigma.Encipher(filename, outfilename)

        print("Файл зашифрован и сохранен под именен {0}!".format(outfilename))
        if conf is None:
            ConfPars.SaveConfig(textEnigma.GetEnigmaConfig(), "config/text")
            print("Настройки Энигмы успешно сохранены в файл", "config/text")

    except EnvironmentError:
        print("Ошибка при открытии файла!")


def EncipherBinary(filename, conf, outfilename):
    enigma = Enigma(bin_alphabet)

    if conf is not None:
        enigmaConfig = ConfPars.LoadConfig(conf)
        enigma.SetConfig(*enigmaConfig)
        print("Настройки Энигмы успешно загружены из файла", conf)

    binEnigma = BinEnigma(enigma)

    try:
        binEnigma.Encipher(filename, outfilename)

        print("Файл зашифрован и сохранен под именен {0}!".format(outfilename))
        if conf is None:
            ConfPars.SaveConfig(binEnigma.GetEnigmaConfig(), "config/bin")
            print("Настройки Энигмы успешно сохранены в файл", "config/bin")

    except EnvironmentError:
        print("Ошибка при открытии файла!")


def parse_args():
    parser = argparse.ArgumentParser(description="Энигма для шифрования строк,"
                                     + " текстовых и бинарных файлов")
    subparsers = parser.add_subparsers(help="режим работы программы")

    parser_str = subparsers.add_parser('str', help='шифрование строк')
    parser_str.add_argument('--conf', nargs='?',
                        help='указание файла с начальными настройками Энигмы')
    parser_str.set_defaults(mode='str')

    parser_text = subparsers.add_parser('text',
                                        help='шифрование текстового файла')
    parser_text.add_argument('filename', type=str, help='путь к файлу')
    parser_text.add_argument('--conf', nargs='?',
                        help='указание файла с начальными настройками Энигмы')
    parser_text.add_argument('--out', nargs='?', help='указание имени выходного файла')
    parser_text.set_defaults(mode='text')

    parser_bin = subparsers.add_parser('bin', help='шифрование бинарного файла')
    parent_bin = parser_bin.add_argument('filename', type=str,
                             help='путь к файлу')
    parser_bin.add_argument( '--conf', nargs='?',
                    help='указание файла с начальными настройками Энигмы')
    parser_bin.add_argument('--out', nargs='?', help='указание имени выходного файла')
    parser_bin.set_defaults(mode='bin')

    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    if args.mode == "str":
        EncipherString(args.conf)

    elif args.mode == "text":
        EncipherText(args.filename, args.conf,
                     args.out if args.out is not None else"result.txt")

    elif args.mode == "bin":
        EncipherBinary(args.filename, args.conf,
                       args.out if args.out is not None else "result.bin")

