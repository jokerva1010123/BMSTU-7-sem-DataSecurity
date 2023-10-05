import os
from bitarray import bitarray
from bitarray.util import ba2hex, hex2ba
import pyfiglet

import config
from utils import printProgressBar


SPACE = "  "
RESULT_FOLDER = "result/"
BYTES_PER_ITERATION = 100

class Node():
    """
        Звено дерева для алгоритма сжатия Хаффмана
    """
    def __init__(self, count: int, data=None, left=None, right=None):
        self.data = data
        self.count = count
        self.left = left
        self.right = right

    def getData(self):
        return self.data

    def getCount(self):
        return self.count

    def printNode(self):
        print("{0}: {1}".format(self.data, self.count))


class HaffmanCode():
    """
        Алгоритм сжатия Хаффмана
    """
    def __init__(self) -> None:
        self.root = None

    def __translateDictToNodeList(self, dictionary: dict):
        """
            Перевод словаря в список Node
        """
        return [Node(v, k) for k, v in dictionary.items()]


    def __sortNodeList(self, nodes: list[Node], reverse=False):
        """
            Сортировка списка Node
        """
        return sorted(nodes, key=lambda item: item.count, reverse=reverse)


    def generateTreeByDict(self, frequencyTable: dict):
        """
            Генерация дерева для алгоритма Хаффмана по
            словарю {символ: частота встречи}
        """
        nodeList = self.__translateDictToNodeList(frequencyTable)

        while (len(nodeList) != 1):
            nodeList = self.__sortNodeList(nodeList, reverse=True)

            firstElem = nodeList.pop()
            secondElem = nodeList.pop()

            summarisedElem = Node(
                # data=firstElem.getData() + secondElem.getData(),
                count=firstElem.getCount() + secondElem.getCount(),
                left=secondElem,
                right=firstElem,
            )

            nodeList.append(summarisedElem)

        self.root = nodeList[0]


    def __getCodeBySymbolRecursive(self, root: Node, symbol, code):
        """
            Получить код символа в дереве (рекурсивно)
        """
        result = ""

        if (root.data == symbol):
            # print("{}: {}".format(symbol, code))
            result += code

        if (root.left):
            result += self.__getCodeBySymbolRecursive(root.left, symbol, code + "0")

        if (root.right):
            result += self.__getCodeBySymbolRecursive(root.right, symbol, code + "1") 

        return result

    
    def getCodeBySymbol(self, symbol):
        """
            Получить код символа в дереве
        """
        if (self.root == None):
            print("Ошибка: Дерево не сгенерировано")
            return

        return self.__getCodeBySymbolRecursive(self.root, symbol=symbol, code="")


    def __getSymbolByCodeRecursive(self, root: Node, code):
        """
            Получить символ по его коду в дереве (рекурсивно)
        """
        result = ""

        if (code == ""):
            result = root.getData()

        if (code[:1] == "0"):
            result = self.__getSymbolByCodeRecursive(root.left, code[1:])

        if (code[:1] == "1"):
            result = self.__getSymbolByCodeRecursive(root.right, code[1:])

        return result


    def getSymbolByCode(self, code):
        """
            Получить символ по его коду в дереве
        """
        if (self.root == None):
            print("Ошибка: Дерево не сгенерировано")
            return

        return self.__getSymbolByCodeRecursive(self.root, code)


    def getRoot(self):
        """
            Получить корень дерева
        """
        return self.root


    def printTree(self, space: str):
        """
            Распечатать дерево
        """
        self.__printTreeRecursive(self.root, space=space)


    def __printTreeRecursive(self, root: Node, space: str):
        """
            Распечатать дерево рекурсивно
        """
        print("{}{}: {}".format(space, root.data, root.count))

        if (root.left):
            self.__printTreeRecursive(root.left, space + SPACE)

        if (root.right):
            self.__printTreeRecursive(root.right, space + SPACE)


def getFrequencyTable(filePath: str) -> dict:
    """
        Получение таблицы частотности встречи каждого
        байта в файле filePath
    """
    try:
        fullPath = os.path.join(os.getcwd(), filePath)
        file = open(fullPath, "rb")
    except:
        print("Ошибка: файл \'%s\' отстуствует" % fullPath)
        return

    frequencyTable = dict()

    print("\nВычисление таблицы частотности встречи байтов в файле")

    ###### Для шкалы выполнения процесса ######
    fileSize = file.seek(0, os.SEEK_END)
    file.seek(0)

    curIteration = 0
    totalIterations = fileSize
    ###### Для шкалы выполнения процесса ######

    while True:
        byte = file.read(1)

        if not byte:
            break

        count = 1
        
        if (byte in frequencyTable):
            count = frequencyTable.get(byte) + 1
        
        frequencyTable.update({byte: count})

        curIteration += 1
        printProgressBar(curIteration, totalIterations, prefix=" Прогресс", suffix="Выполнено")

    file.close()

    return frequencyTable


def compressFile(srcPath: str, dstPath: str, frequencdyTable: dict):
    """
        Функция сжимает файл srcPath и записывает результат в dstPath
        При этом, если кол-во битов не делится на 8, то дополняется
        0-ыми битами и их количество записывается в конец файла (по коду Хаффмана)
    """

    haffmanCode = HaffmanCode()
    haffmanCode.generateTreeByDict(frequencdyTable)

    ################# READ #######################
    try:
        fullPath = os.path.join(os.getcwd(), srcPath)
        srcFile = open(fullPath, "rb")
    except:
        print("Ошибка: файл \'%s\' отстуствует" % fullPath)
        return

    print("\nСжатие файла")
    ###### Для шкалы выполнения процесса ######
    fileSize = srcFile.seek(0, os.SEEK_END)
    srcFile.seek(0)

    curIteration = 0
    totalIterations = fileSize
    ###### Для шкалы выполнения процесса ######

    resultStringOfBits = ""

    while True:
        byte = srcFile.read(1)

        if not byte:
            break
        
        code = haffmanCode.getCodeBySymbol(byte)

        if (code == ""):
            print("Ошибка: Неверный символ")
            return

        resultStringOfBits += code

        curIteration += 1
        printProgressBar(curIteration, totalIterations, prefix=" Прогресс", suffix="Выполнено")

    srcFile.close()
    ################# READ #######################

    ################ ADD BITS #####################
    extraBits = len(resultStringOfBits) % 8

    if (extraBits != 0):
        bitsToAdd = 8 - extraBits
        resultStringOfBits += "0" * bitsToAdd
    ################ ADD BITS #####################

    ############### WRITE BITS ####################
    fullPath = os.path.join(os.getcwd(), dstPath)
    
    with (open(fullPath, "wb")) as dstFile:
        bytesToWrite = bitarray(resultStringOfBits).tobytes()
        dstFile.write(bytesToWrite)

        dstFile.write(bytes(str(bitsToAdd), encoding="utf-8"))
    ############### WRITE BITS ####################


def decompressFile(srcPath: str, dstPath: str, frequencdyTable: dict):
    """
        Функция возвращает исходный файл из сжатого srcPath и записывает в dstPath
        При этом сначала вычитаются лишние биты, исходя из числа в крайнеа байте
        файла srcPath (по коду Хаффмана)
    """

    haffmanCode = HaffmanCode()
    haffmanCode.generateTreeByDict(frequencdyTable)

    ################# READ #######################
    try:
        fullPath = os.path.join(os.getcwd(), srcPath)
        srcFile = open(fullPath, "rb")
    except:
        print("Ошибка: файл \'%s\' отстуствует" % fullPath)
        return

    resultBytes = srcFile.read()
    srcFile.close()
    ################# READ #######################

    ############## DELETE BITS ###################
    bitsToDelete = int(resultBytes[-1:].decode(encoding="utf-8"))
    resultBytes = resultBytes[:-1]

    bits = hex2ba(resultBytes.hex()).to01() # в тип данных строка!
    bits = bits[:-bitsToDelete]
    ############## DELETE BITS ###################

    ############### WRITE BITS ###################
    fullPath = os.path.join(os.getcwd(), dstPath)
    dstFile = open(fullPath, "wb")

    print("\nВосстановление файла")
    ###### Для шкалы выполнения процесса ######
    curIteration = 0
    totalIterations = len(bits)
    ###### Для шкалы выполнения процесса ######

    code = ""

    # берется по одному числа кода и ищется символ
    # не нашелся -- берется еще одно число кода
    while (bits != ""): 
        code += bits[:1]
        bits = bits[1:]

        result = haffmanCode.getSymbolByCode(code)

        if (result != None):
            dstFile.write(result)
            code = ""

        curIteration += 1
        printProgressBar(curIteration, totalIterations, prefix=" Прогресс", suffix="Выполнено")

    dstFile.close()
    ############### WRITE BITS ###################


def parseFile(filePath: str = config.defaultTestProgram):
    frequencyTable = getFrequencyTable(filePath)
    
    compressedFilePath = RESULT_FOLDER + config.compressedFile
    compressFile(filePath, compressedFilePath, frequencyTable)

    decompressedFilePath = RESULT_FOLDER + config.decompressedFile
    decompressFile(compressedFilePath, decompressedFilePath, frequencyTable)

    print("\nПроцесс завершен. \
        \n\tВходной файл:         \'{0}\'; \
        \n\tСжатый файл:          \'{1}\' \
        \n\tВосстановленный файл: \'{2}\'"
            .format(filePath, compressedFilePath, decompressedFilePath))


def main():
    title = pyfiglet.figlet_format("Haffman Code")
    print(title)

    filePath = input("Введите путь до файла (по умолчанию \'{}\'): "
        .format(config.defaultTestProgram))

    if (filePath == ""):
        parseFile()
    else:
        parseFile(filePath)


if __name__ == "__main__":
    main()
