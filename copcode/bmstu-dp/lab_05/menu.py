from dataclasses import dataclass


@dataclass
class Menu:
    TEXT = '''
Электронная подпись

Выберите пункт меню:
  1 - Подписание файла
  2 - Проверка подписи
  0 - Выход

Выбор: '''

    SIGNING = 1
    VERIFICATION = 2
    EXIT = 0
