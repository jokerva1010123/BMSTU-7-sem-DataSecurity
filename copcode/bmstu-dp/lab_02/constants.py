from dataclasses import dataclass


@dataclass
class Settings:
    direct = 0
    reverse = 1

    random_filling = 1
    file_filling = 2

    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
