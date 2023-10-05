from dataclasses import dataclass


@dataclass
class Menu:
    TEXT = '''
Шифрование алгоритмом RSA

Выберите пункт меню:
  1 - Зашифровать файл
  2 - Расшифровать файл
  0 - Выход

Выбор: '''

    ENCIPHERING = 1
    DECIPHERING = 2
    EXIT = 0
