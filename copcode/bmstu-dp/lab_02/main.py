from os.path import exists, isfile
from dataclasses import dataclass

from enigma.enigma import Enigma
from constants import Settings


@dataclass
class Menu:
    msg = '''
Выберите пункт меню:

1 - Зашифровать файл
2 - Расшифровать файл
3 - Зашифровать сообщение
4 - Расшифровать сообщение
0 - Выход
'''

    encipher_file = 1
    decipher_file = 2
    encipher_msg = 3
    decipher_msg = 4
    exit = 0


def encipher_msg():
    plain_msg = input('Введите сообщение: ')

    alphabet = Settings.alphabet
    enigma = Enigma(alphabet, Settings.random_filling)

    encipher_msg = ''

    for letter in plain_msg:
        encipher_msg += str(enigma.encipher(letter))

    print('Зашифрованное сообщение: ', encipher_msg)

    enigma.save_settings()
    return


def decipher_msg():
    encipher_msg = input('Введите сообщение: ')

    alphabet = Settings.alphabet
    enigma = Enigma(alphabet, Settings.file_filling)

    plain_msg = ''

    for letter in encipher_msg:
        plain_msg += enigma.encipher(letter)

    print('Расшифрованное сообщение: ', plain_msg)

    return


def encipher_file():
    name_input = input('Введите имя файла: ')

    if (not exists(name_input) or not isfile(name_input)):
        print('Файл не найден.')
        return

    name_result = 'encipher.' + name_input.split('.')[1]

    alphabet = bytes([letter for letter in range(256)])
    enigma = Enigma(alphabet, Settings.random_filling)
    result_file = open(name_result, 'wb')

    with open(name_input, 'rb') as input_file:
        while True:
            byte = input_file.read(1)

            if not byte:
                break

            line = bytes([enigma.encipher(byte)])
            result_file.write(line)
    
    result_file.close()

    enigma.save_settings()

    print('Файл зашифрован в файл {}.'.format(name_result))
    return


def decipher_file():
    name_input = input('Введите имя файла: ')

    if (not exists(name_input) or not isfile(name_input)):
        print('Файл не найден.')
        return
    
    name_result = 'result.' + name_input.split('.')[1]

    alphabet = bytes([letter for letter in range(256)])
    enigma = Enigma(alphabet, Settings.file_filling)

    result_file = open(name_result, 'wb')

    with open(name_input, 'rb') as input_file:
        while True:
            byte = input_file.read(1)

            if not byte:
                break

            line = bytes([enigma.encipher(byte)])
            result_file.write(line)
    
    result_file.close()

    print('Файл расшифрован в файл {}.'.format(name_result))
    return


if __name__ == "__main__":
    run = True

    while run:
        print(Menu.msg)
        command = int(input('Выбор: '))

        if command == Menu.encipher_file:
            encipher_file()
        elif command == Menu.decipher_file:
            decipher_file()
        elif command == Menu.encipher_msg:
            encipher_msg()
        elif command == Menu.decipher_msg:
            decipher_msg()
        elif command == Menu.exit:
            run = False
        else:
            print('Выбран неверный пункт меню.')
