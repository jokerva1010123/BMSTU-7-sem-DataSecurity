from bitarray.util import hex2ba, ba2hex

from des import des, fillZero
from defines import *
import config


def encryptHex(messageHex: str, keyHex: str, operationType: int) -> str:
    '''
        Шиврование/расшифровка hex-строки messageHex по алгоритму DES с ключом keyHex
    '''
    keyBin = fillZero(hex2ba(keyHex), MESSAGE_SIZE)

    # Дешифрование
    message16hex = str() # 16 hex in 64 bits
    encryptedHex = str()

    for symbol in messageHex:
        if (len(message16hex) != 16):
            message16hex += symbol
        else:
            messageBin = fillZero(hex2ba(message16hex), MESSAGE_SIZE) # fillZero по идее, не нужна, но пусть будет

            encrypted = des(messageBin, keyBin, operationType=operationType)
            encryptedHex += ba2hex(encrypted)

            message16hex = symbol

    if (len(message16hex) != 0): # если остались необработанные символы
        messageBin = fillZero(hex2ba(message16hex), MESSAGE_SIZE)

        encrypted = des(messageBin, keyBin, operationType=operationType)
        encryptedHex += ba2hex(encrypted)

    return encryptedHex


def encodeFile(srcFileName: str, dstFileName: str, keyHex: str, operationType: int):
    '''
        Шиврование/расшифровка файла srcFileName по алгоритму DES с ключом keyHex
    '''
    try:
        srcFile = open(srcFileName, "rb")
    except FileNotFoundError:
        print("\nФайл \'{}\' не существует".format(srcFileName))
        return NO_FILE

    dstFile = open(dstFileName, "wb")

    keyBin = fillZero(hex2ba(keyHex), MESSAGE_SIZE)

    while True:
        readBytes8 = srcFile.read(8)

        if not readBytes8:
            break

        readHex = readBytes8.hex()
        readBytesBin = fillZero(hex2ba(readHex), MESSAGE_SIZE)
        encryptBin = des(readBytesBin, keyBin, operationType=operationType)
        encryptHex = ba2hex(encryptBin)

        encryptBytes = bytes.fromhex(encryptHex)
        dstFile.write(encryptBytes)

    srcFile.close()
    dstFile.close()

    if (operationType == CYPHER):
        print("\nФайл зашифрован. Входной файл: \'{0}\'; выходной файл: \'{1}\'"
            .format(srcFileName, dstFileName))
    elif (operationType == DECYPHER):
        print("\nФайл расшифрован. Входной файл: \'{0}\'; выходной файл: \'{1}\'"
            .format(srcFileName, dstFileName))
    else:
        print("\nНеизвестная опция работы")

    return OK


def parseHex(messageHex: str, keyHex: str = config.keyHex):
    '''
        Шифрование и расшифровка hex-строки messageHex функцией encryptHex с ключом keyHex
    '''
    # Данные
    try: 
        messageBin = hex2ba(messageHex)
    except:
        print("Строка \'{}\' имеет не HEX формат".format(messageHex))
        return

    print("Оригинальное HEX-сообщение:", messageHex)

    # Шифровка
    encrypted = encryptHex(messageHex, keyHex, operationType=CYPHER)
    print("Encrypted:", encrypted)

    # Дешифровка
    decrypted = encryptHex(encrypted, keyHex, operationType=DECYPHER)
    print("Decrypted:", decrypted)


def parseText(message: str, keyHex: str = config.keyHex):
    '''
        Шифрование и расшифровка строки message функцией encryptHex с ключом keyHex
    '''
    print("\nОригинальное сообщение:", message)
    messageHex = "".join(["{:x}".format(ord(s)) for s in message])

    encryptedHex = encryptHex(messageHex, keyHex, operationType=CYPHER)
    print("\nEncrypted HEX:", encryptedHex)

    decryptedHex = encryptHex(encryptedHex, keyHex, operationType=DECYPHER)
    decryptedStr = bytes.fromhex(decryptedHex).decode("utf-8")
    print("Decrypted:", decryptedStr)


def parseFile(srcFileName: str = config.defaultTestProgram, keyHex: str = config.keyHex):
    '''
        Шифрование и расшифровка файла srcFileName функцией encryptFile с ключом keyHex
    '''
    encodedFilePath = RESULT_FOLDER_PATH + config.encodedFile
    returnCode = encodeFile(srcFileName, encodedFilePath, keyHex, operationType=CYPHER)

    if (returnCode != OK):
        return

    decodedFilePath = RESULT_FOLDER_PATH + config.decodedFile
    returnCode = encodeFile(encodedFilePath, decodedFilePath, keyHex, operationType=DECYPHER)


def main():
    menuText = '''
        Алгоритм шифрования DES

    1. Зашифровать строку
    2. Зашифровать HEX
    3. Зашифровать файл

    0. Выход
    '''

    option = -1

    while (option != 0):
        print(menuText)

        try:
            option = int(input("Выбор: "))
        except:
            print("\nПункт введен неверно")
            continue

        print(BREAKLINE)

        if (option == 1):
            string = input("Введите строку для шифрования: ")
            parseText(string)
        elif (option == 2):
            hex = input("Введите HEX для шифрования: ")
            parseHex(hex)
        elif (option == 3):
            filePath = input("Введите путь до файла (по умолчанию \'{}\'): "
                .format(config.defaultTestProgram))

            if (filePath == ""):
                parseFile()
            else:
                parseFile(filePath)
        elif(option == 0):
            break
        else:
            print("\nПункт выбран неверно")

        print(BREAKLINE)


if __name__ == "__main__":
    main()
