from defines import *
import config as cfg

from rsa import getKeys, rsaCypher


def parseNumber(number: int):
    """
        Шифровка/расшифровка числа алгоритмом RSA
    """
    keyD, keyE = getKeys()

    print("\nПубличный ключ E:", keyE)
    print("Секретный ключ D:", keyD)

    encoded = rsaCypher(number, keyE)
    print("\nЭлемент зашифрован:", encoded)

    decoded = rsaCypher(encoded, keyD)
    print("Элемент расшифрован:", decoded)


def encodeBinary(pathSrc: str, pathDst: str, key: list[int], operationType: int):
    """
        Шифровка/расшифровка файла побайтово алогритмом RSA
    """
    try:
        srcFile = open(pathSrc, "rb")
    except FileNotFoundError:
        print("\nОшибка: файл \'{0}\' не существует. Шифрование невозможно".format(pathSrc))
        return -1
    
    dstFile = open(pathDst, "wb")

    while True:
        if (operationType == CYPHER):
            byte = srcFile.read(1)
        elif (operationType == DECYPHER):
            byte = srcFile.read(4)

        if not byte:
            break
        
        byteInteger = int.from_bytes(bytes=byte, byteorder="little")
        encodedInt = rsaCypher(byteInteger, key)

        if (operationType == CYPHER):
            encodedByte = encodedInt.to_bytes(4, byteorder="little")
            dstFile.write(encodedByte)
        elif (operationType == DECYPHER):
            encodedByte = encodedInt.to_bytes(1, byteorder="little")
            dstFile.write(encodedByte)

    srcFile.close()
    dstFile.close()

    if (operationType == CYPHER):
        print("\nФайл зашифрован. Входной файл: \'{0}\'; выходной файл: \'{1}\'"
            .format(pathSrc, pathDst))
    elif (operationType == DECYPHER):
        print("\nФайл расшифрован. Входной файл: \'{0}\'; выходной файл: \'{1}\'"
            .format(pathSrc, pathDst))
    else:
        print("\nНеизвестная опция работы")

    return 0


def parseFile(srcFilePath: str = cfg.defaultTestProgram):
    """
        Шифровка/расшифровка файла функцией encodeBinary
    """
    keyD, keyE = getKeys()

    print("\nПубличный ключ E:", keyE)
    print("Секретный ключ D:", keyD)

    encodedFilePath = RESULT_FOLDER_PATH + cfg.encodedFile
    encodeBinary(srcFilePath, encodedFilePath, key=keyE, operationType=CYPHER)

    decodedFilePath = RESULT_FOLDER_PATH + cfg.decodedFile
    encodeBinary(encodedFilePath, decodedFilePath, key=keyD, operationType=DECYPHER)


def main():
    """
        Основная функция
    """
    
    menuText = '''
        Алгоритм шифрования RSA

    1. Зашифровать число
    2. Зашифровать файл

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
            number = int(input("Введите число для шифрования: "))
            parseNumber(number)
        elif (option == 2):
            filePath = input("Введите путь до файла (по умолчанию \'{}\'): "
                .format(cfg.defaultTestProgram))

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
