import hashlib

from defines import *
import config as cfg

from rsa.rsa import getKeys, rsaCypher


def hashFile(srcFilePath: str, hashFilePath: str):
    try:
        srcFile = open(srcFilePath, "rb")
    except FileNotFoundError:
        print("\nОшибка: файл \'{0}\' не существует. Хэширование невозможно".format(srcFilePath))
        return -1

    dstFile = open(hashFilePath, "w")

    text = srcFile.read()
    hashedBytes = hashlib.sha512(text).hexdigest()
    dstFile.write(hashedBytes)

    srcFile.close()
    dstFile.close()

    print("\nФайл \'{0}\' захеширован и результат записан в файл \'{1}\'"
        .format(srcFilePath, hashFilePath))


def encodeBinary(pathSrc: str, pathDst: str, key: list[int], operationType: int):
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


# НЕПРАВИЛЬНО, НАДО ТАК: decodedFile, public key, srcFile
# 1. Расшифроать decodedFile (с исп public key) -> hash
# 2. Захешировать srcFile
# 3. Сравнить хеши


def compareFiles(oneFilePath: str, twoFilePath: str):
    print("\n" + "-" * 30 + "СРАВНЕНИЕ ФАЙЛОВ" + "-" * 30)
    print("Файлы:\n\t1. \'{0}\'\n\t2. \'{1}\'\n".format(oneFilePath, twoFilePath))

    try:
        oneFile = open(oneFilePath, "rb")
    except FileNotFoundError:
        print("\nОшибка: файл \'{0}\' не сущетсвует. Шифрование невозможно".format(oneFilePath))
        return -1

    try:
        twoFile = open(twoFilePath, "rb")
    except FileNotFoundError:
        print("\nОшибка: файл \'{0}\' не сущетсвует. Шифрование невозможно".format(twoFilePath))
        return -1

    if (oneFile.read() == twoFile.read()):
        print("РЕЗУЛЬТАТ: Файлы идентичны")
    else:
        print("РЕЗУЛЬТАТ: Файлы разные")

    print("-" * 30 + "СРАВНЕНИЕ ФАЙЛОВ" + "-" * 30)

    oneFile.close()
    twoFile.close()



def parseFile(srcFilePath: str):
    keyD, keyE = getKeys()

    print("\nПубличный ключ E:", keyE)
    print("Секретный ключ D:", keyD)

    # Изменение: для электронной подписи ключи меняются местами
    encodedFilePath = RESULT_FOLDER_PATH + cfg.encodedFile
    encodeBinary(srcFilePath, encodedFilePath, key=keyD, operationType=CYPHER)

    decodedFilePath = RESULT_FOLDER_PATH + cfg.decodedFile
    encodeBinary(encodedFilePath, decodedFilePath, key=keyE, operationType=DECYPHER)

    compareFiles(srcFilePath, decodedFilePath)





def main():
    menuText = '''
        Электронная подпись

    1. Получить хэш-файл файла подписи
    2. Зашифровать/расшифровать хэш подпись

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
            filePath = input("Введите путь до файла (по умолчанию \'{}\'): "
                .format(cfg.defaultSignFile))

            hashFilePath = RESULT_FOLDER_PATH + cfg.signHashFile

            if (filePath == ""):
                hashFile(cfg.defaultSignFile, hashFilePath)
            else:
                hashFile(filePath, hashFilePath)


        elif (option == 2):
            filePath = input("Введите путь до файла (по умолчанию \'{}\'): "
                .format(RESULT_FOLDER_PATH + cfg.signHashFile))

            if (filePath == ""):
                parseFile(RESULT_FOLDER_PATH + cfg.signHashFile)
            else:
                parseFile(filePath)
        elif(option == 0):
            break
        else:
            print("\nПункт выбран неверно")

        print(BREAKLINE)


if __name__ == "__main__":
    main()
