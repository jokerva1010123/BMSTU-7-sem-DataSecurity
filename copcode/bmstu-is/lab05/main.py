import sys
import os
import argparse

from typing import Tuple

import logging
logging.basicConfig(filename="ds.log", filemode="w", level=logging.DEBUG)

from termcolor import cprint

import hashlib

from myrsa.generation import GenerateKeys
from myrsa.myrsa import Encrypt, Decrypt

def parse_args():
    parser = argparse.ArgumentParser(description="Создание цифровой подписи")
    subparsers = parser.add_subparsers(help="режим работы программы")
    subparsers.required = True

    parser_gen = subparsers.add_parser('gen', help='генерация ключей')
    parser_gen.set_defaults(mode='gen')

    parser_dsgen = subparsers.add_parser('dsgen', help='создание подписи')
    parent_dsgen = parser_dsgen.add_argument('filename', type=str, help='путь к файлу')
    parser_dsgen.add_argument('e', help='часть закрытого ключа e', type=int)
    parser_dsgen.add_argument('N', help='часть закрытого ключа N', type=int)

    parser_dsgen.set_defaults(mode='dsgen')

    parser_dschk = subparsers.add_parser('dscheck', help='проверка подписи')
    parent_dschk = parser_dschk.add_argument('filename', type=str, help='путь к файлу')
    parser_dschk.add_argument('dsfile', help='путь к подписи', type=str)
    parser_dschk.add_argument('d', help='часть открытого ключа d', type=int)
    parser_dschk.add_argument('N', help='часть открытого ключа N', type=int)

    parser_dschk.set_defaults(mode='dscheck')

    return parser.parse_args()


def HashFile(file: str):
    digsig = None
    with open(file, "rb") as f:
        byte = f.read()
        digsig = hashlib.sha256(byte).digest()

    return digsig


def GenerateDigitalSignature(file: str, key: Tuple[int, int]) -> None:
    hashValue = HashFile(file)
    with open("./encrypted/digsig", "wb") as fileOut:
        for byte in hashValue:
            encrypted = Encrypt(byte, key)
            fileOut.write(encrypted.to_bytes(8, "big"))


def CheckDigitalSignature(file: str, dsfile:str, key: Tuple[int, int]):
    hashValue = HashFile(file)
    recievedHash = b''
    with open(dsfile, "rb") as digsigFile:
        while (byte := digsigFile.read(8)):
            number = int.from_bytes(byte, "big")
            decrypted = Decrypt(number, key)
            recievedHash += decrypted.to_bytes(1, "big")

    return hashValue == recievedHash


if __name__ == "__main__":
    args = parse_args()

    if args.mode == "gen":
        logging.info("KEYS GENERATION")
        logging.debug(f"args {args}")
        publicKey, privateKey = GenerateKeys(1000000)

        cprint(f"Открытый ключ: {publicKey[0]} {publicKey[1]}", "cyan")
        cprint(f"Закрытый ключ: {privateKey[0]} {privateKey[1]}", "cyan")
        cprint("Не сообщайте никому закрытый ключ!!!", "red")

    elif args.mode == "dsgen":
        logging.info("GENERATE DIGITAL SIGNATURE")
        logging.debug(f"args {args}")

        GenerateDigitalSignature(args.filename, (args.e, args.N))

        endPath = os.path.basename(args.filename)
        cprint(f"Цифровая подпись успешно создана и сохранена по пути "
               f"./encrypted/digsig", "green")

    elif args.mode == "dscheck":
        logging.info("CHECK DIGITAL SIGNATURE")
        logging.debug(f"args {args}")

        res = CheckDigitalSignature(args.filename, args.dsfile, (args.d, args.N))

        if res:
            cprint("Подпись верна!", "green")
        else:
            cprint("Обнаружено несовпадение!", "red")

