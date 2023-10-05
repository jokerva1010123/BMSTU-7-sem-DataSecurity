from bitarray import bitarray
from bitarray.util import hex2ba, ba2hex

from defines import *


def fillZero(bitArr: bitarray, size: int) -> bitarray:
    '''
        Дополняет bitArr нулями в начале до размера size
    '''
    return bitarray("0" * (size - len(bitArr))) + bitArr


def permutation(bitArr: bitarray, tablePermutation: list) -> bitarray:
    '''
        Перестановка bitArr на основании таблицы перестановок tablePermutation
    '''
    return bitarray([bitArr[symbol - 1] for symbol in tablePermutation])


def cycleShiftN(bitArr: bitarray, postions: int) -> bitarray:
    '''
        Циклический сдвиг bitArr влево на postions позиций
    '''
    return (bitArr << postions) | fillZero(bitArr[:postions], len(bitArr))


def generateKeys(key: bitarray) -> list[bitarray]:
    '''
        Генерация списка из 16 раундовых ключей на основании заданного ключа key
    '''
    if (len(key) % MESSAGE_SIZE != 0):
        print("Неверный размер ключа! Кодирование невозможно")
        return

    # 1. Начальная перестановка (преобразование ключа из 64 бит в 56 бит)
    permutedBitArr = permutation(key, B)

    # 2. Разбить на два блока по 28 бит
    blockC = permutedBitArr[:28]
    blockD = permutedBitArr[28:]

    # 3. Генерация ключей
    keys = list()

    for round in range(ROUNDS):
        # 3.1 Циклический сдвиг блоков на число, 
        # полученное из SI для конкретного раунда
        shiftedBlockC = cycleShiftN(blockC, SI[round])
        shiftedBlockD = cycleShiftN(blockD, SI[round])

        # 3.2 Соединение блоков
        keyRound = shiftedBlockC + shiftedBlockD

        # 3.3 Сжимающая перестановка CP (56->48)
        compressedKey = permutation(keyRound, CP)

        # 3.4 Добавить получившийся ключ в массив ключей
        keys.append(compressedKey)
        # print("Key[", round + 1, "] =", compressedKey, len(compressedKey))

    return keys


def sBlockParse(bitArr: bitarray, sBlock: list) -> bitarray:
    '''
        Преобразование 6 бит bitArr в 4 бита по таблице sBlock
    '''

    # 1. Строка: Сумма 1-ого и крайнего битов
    row = bitArr[0] + bitArr[-1]
    column = int(ba2hex(bitArr[1:-1]), 16)

    # 2. Столбец: 2-4 биты bitArr
    sBlockNumber = sBlock[row][column]
    binNum = hex2ba('{:x}'.format(sBlockNumber))

    return binNum


def feistel(bitArr: bitarray, key: bitarray) -> bitarray:
    '''
        Функция шифрования Фейстеля
    '''
    # 1. Расширяющая перестановка E (32->48)
    extendedBitArr = permutation(bitArr, E)

    # 2. XOR расширенного сообщения и ключа
    z = extendedBitArr ^ key

    # 3. Формирование выходного сообщения (48->32)
    message = bitarray()

    for i in range(8):
        # 3.1 Получить число из S[i] (6->4)
        z6bitsN = z[i * 6:(i + 1) * 6]
        sBlockNumN = sBlockParse(z6bitsN, S[i])

        message += sBlockNumN

    # 4. Финальная перестановка P (32->32)
    finalPermut = permutation(message, P)

    return finalPermut


def des(message: bitarray, key: bitarray, operationType: int):
    '''
        Функция шифрования DES
    '''
    if (len(message) % MESSAGE_SIZE != 0):
        print("Неверный размер сообщения! Кодирование невозможно")
        return

    # 1. Генерация 16 раундовых ключей
    keys = generateKeys(key)

    if (operationType == DECYPHER):
        keys = list(reversed(keys))

    # 2. Начальная перестановка IP (64->64)
    permutedMessage = permutation(message, IP)

    # 3. Разделение на два блока
    blockLeft = permutedMessage[:32]
    blockRight = permutedMessage[32:]

    if (operationType == DECYPHER):
        blockLeft, blockRight = blockRight, blockLeft 

    # 4. Цикл по 16 раундам
    for round in range(ROUNDS):
        # 4.1 XOR левого блока и функции Фейстеля
        changedRight = blockLeft ^ feistel(blockRight, keys[round])

        # 4.2 Обмен блоков
        blockLeft = blockRight
        blockRight = changedRight

    if (operationType == DECYPHER):
        blockLeft, blockRight = blockRight, blockLeft 

    # 5. Финальная перестановка
    finalPermut = permutation(blockLeft + blockRight, IP1)

    return finalPermut


# chmod u+x encoded.bin
# chmod u+x decoded.bin
