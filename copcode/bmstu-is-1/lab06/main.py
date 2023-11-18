import sys
import os
import argparse

from typing import Tuple

import logging
logging.basicConfig(filename="huffman.log", filemode="w", level=logging.DEBUG)

from termcolor import cprint

from fileio.fileio import FileIO
from huffman.coder import HuffmanCoder


def print_error(msg: str):
    cprint("ОШИБКА:", "red", end=" ")
    print(msg)


def parse_args():
    parser = argparse.ArgumentParser(
                 description="Сжатие файлом методом Хаффмана")
    subparsers = parser.add_subparsers(help="режим работы программы")
    subparsers.required = True

    parser_enc = subparsers.add_parser('comp', help='сжатие файла')
    parent_enc = parser_enc.add_argument('filename', type=str, help='путь к файлу')
    parser_enc.set_defaults(mode='comp')

    parser_dec = subparsers.add_parser('decomp', help='распаковка файла')
    parent_dec = parser_dec.add_argument('filename', type=str, help='путь к файлу')
    parser_dec.set_defaults(mode='decomp')

    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    if args.mode == "comp":
        logging.info("COMPRESSING")
        logging.debug(f"args {args}")

        fileWorker = FileIO()
        b = fileWorker.ReadToBytes(args.filename)
        coder = HuffmanCoder()
        compressed = coder.Compress(b)

        endPath = os.path.basename(args.filename)
        fileWorker.WriteBitarray(f"./compressed/{endPath}", compressed)

        cprint(f"Файл успешно сжат и сохранен по пути "
               f"./compressed/{endPath}", "green")

    elif args.mode == "decomp":
        logging.info("DECOMPRESSING")
        logging.debug(f"args {args}")

        fileWorker = FileIO()
        b = fileWorker.ReadToBits(args.filename)
        coder = HuffmanCoder()
        decompressed = coder.Decompress(b)

        endPath = os.path.basename(args.filename)
        fileWorker.WriteBytes(f"./decompressed/{endPath}", decompressed)

        endPath = os.path.basename(args.filename)
        cprint(f"Файл успешно восстановлен и сохранен по пути "
               f"./decompressed/{endPath}", "green")
