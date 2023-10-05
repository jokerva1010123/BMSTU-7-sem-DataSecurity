from dataclasses import dataclass


@dataclass
class Menu:
    TEXT = '''
Сжатие алгоритмом Хаффмана

Выберите пункт меню:
  1 - Сжать файл
  2 - Восстановить файл
  0 - Выход

Выбор: '''

    COMPRESSING = 1
    DECOMPRESSING = 2
    EXIT = 0
