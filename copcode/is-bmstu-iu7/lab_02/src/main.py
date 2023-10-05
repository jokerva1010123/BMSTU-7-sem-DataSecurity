from random import randint, shuffle
import config

RESULT_FOLDER_PATH = './result/'


class Rotor():
    alphabet: list
    alphabetMixed: list
    position: int


    def __init__(self, alphabet: list, alphabetMixed: list, position: int):
        self.alphabet = alphabet
        self.alphabetMixed = alphabetMixed
        self.position = position


    def setPosition(self, position):
        self.position = position


    def rotate(self):
        self.position = (self.position + 1) % len(self.alphabet)
        return self.position


    def getSymbolForward(self, symbol):
        foundPosition = (self.getSymbolPosition(self.alphabet, symbol) +
            self.position) % (len(self.alphabet))
        foundSymbol = self.alphabetMixed[foundPosition]

        return foundSymbol


    def getSymbolBack(self, symbol):
        foundPosition = (self.getSymbolPosition(self.alphabetMixed, symbol) -
            self.position) % (len(self.alphabet))
        foundSymbol = self.alphabet[foundPosition]

        return foundSymbol


    def getSymbolPosition(self, alphabet: list, symbol):
        curPos = -1

        for i in range(len(alphabet)):
            if (alphabet[i] == symbol):
                curPos = i
                break

        if (curPos == -1):
            print("Такого символа нет в алфавите:", symbol)

        return curPos


class Reflector():
    alphabet: dict

    def __init__(self, alphabet: dict):
        self.alphabet = alphabet


    def reflect(self, symbol):
        return self.alphabet[symbol]


class Enigma():
    alphabet: list
    alphabetReflect: dict
    alphabetMixedN: list
    positionN: list
    rotorCount: int
    rotors: list
    reflector: Reflector


    def __init__(self, alphabet: list, rotorCount: int, alphabetReflect: list = None,
        alphabetMixedN: list = None, positionN: list = None):
        self.alphabet = alphabet.copy()
        self.rotorCount = rotorCount if (rotorCount >= 0) else 0

        if (alphabetReflect is None):
            self.alphabetReflect = self.generateReflectAlphabet().copy()
        else:
            self.alphabetReflect = alphabetReflect.copy()

        if (alphabetMixedN is None):
            self.alphabetMixedN = self.generateMixedAlphabets().copy()
        else:
            if (len(alphabetMixedN) != self.rotorCount):
                print("Ошибка: Кол-во Перемешанных Алфавитов не соответствует кол-ву роторов. \
                    Перемешанные Алфавиты сгенерированы автоматически")
                self.alphabetMixedN = self.generateMixedAlphabets().copy()
            else:
                self.alphabetMixedN = alphabetMixedN.copy()

        if (positionN is None):
            self.positionN = [randint(0, len(self.alphabet)) for _ in range(self.rotorCount)]
        else:
            if (len(positionN) != self.rotorCount):
                print("Ошибка: Кол-во Позиционных Cимволов не соответствует кол-ву роторов. \
                    Позиционные Cимволы сгенерированы автоматически")
                self.positionN = [randint(0, len(self.alphabet)) for _ in range(self.rotorCount)]
                
            self.positionN = positionN.copy()

        self.rotors = [Rotor(self.alphabet, 
                             self.alphabetMixedN[i], 
                             self.positionN[i]) 
                       for i in range(self.rotorCount)]

        self.reflector = Reflector(self.alphabetReflect)
    

    def reset(self):
        for i, rotor in enumerate(self.rotors):
            rotor.setPosition(self.positionN[i])


    def generateMixedAlphabets(self):
        shuffledAlphabets = list()

        for _ in range(self.rotorCount):
            tmpAlphabet = self.alphabet.copy()
            shuffle(tmpAlphabet)
            shuffledAlphabets.append(tmpAlphabet)

        return shuffledAlphabets       


    def generateReflectAlphabet(self):
        tmpReflectAlphabet = self.alphabet.copy()
        shuffle(tmpReflectAlphabet)
        alphabetReflect = dict()

        for i in range(0, len(tmpReflectAlphabet), 2):
            alphabetReflect[tmpReflectAlphabet[i]] = tmpReflectAlphabet[i + 1]
            alphabetReflect[tmpReflectAlphabet[i + 1]] = tmpReflectAlphabet[i]

        return alphabetReflect


    def rotateRotors(self):
        for rotor in self.rotors:
            position = rotor.rotate()

            if (position != 0):
                break


    def process(self, symbol):
        for ind in range(0, len(self.rotors), 1):
            symbol = self.rotors[ind].getSymbolForward(symbol)

        symbol = self.reflector.reflect(symbol)

        for ind in range(len(self.rotors) - 1, -1, -1):
            symbol = self.rotors[ind].getSymbolBack(symbol)

        self.rotateRotors()
        return symbol


    def encodeBinary(self, pathSrc: str, dstName: str):
        try:
            srcFile = open(pathSrc, "rb")
        except FileNotFoundError:
            print("\nОшибка: файл \'{0}\' не сущетсвует. Шифрование невозможно".format(pathSrc))
            return -1
        
        dstFile = open(dstName, "wb")

        while True:
            byte = srcFile.read(1)

            if not byte:
                break

            encodedByte = self.process(byte)
            dstFile.write(bytes(encodedByte))

        srcFile.close()
        dstFile.close()

        return 0


    def encodeStr(self, string: str):
        encryptString = str()

        for symbol in string:
            res = self.process(symbol)
            encryptString += res

        return encryptString
        

def generateBinaryAlphabet():
    alphabet = bytes([code for code in range(256)])
    alphabetList = [bytes([x]) for x in alphabet]
    return alphabetList


def processString(string: str):
    alphabet = config.alphabetString
    enigma = Enigma(alphabet, rotorCount=config.rotorCount)

    encryptString = enigma.encodeStr(string)
    print("Зашифрованная строка: \t", encryptString)
    enigma.reset()
    decryptString = enigma.encodeStr(encryptString)
    print("Расшифрованная строка: \t", decryptString)


def processBinary(fileSrc: str = config.defaultTestProgram):
    alphabetBytes = generateBinaryAlphabet()

    enigma = Enigma(alphabetBytes, rotorCount=config.rotorCount)

    encodedFilePath = RESULT_FOLDER_PATH + config.encodedFile
    returnCode = enigma.encodeBinary(fileSrc, encodedFilePath)

    enigma.reset()

    if (returnCode == 0):
        decodedFilePath = RESULT_FOLDER_PATH + config.decodedFile
        enigma.encodeBinary(encodedFilePath, decodedFilePath)

        print("\nШифрование выполнено успешно. \
            \nЗашифрованный файл -", config.encodedFile,", расшифрованный -", config.decodedFile)



def main():

    menuText = """
      Энигма
    1. Шифрование строки (Верхний регистр Латинского алфавита и символы \"?!. \")
    2. Шифрование любого файла побайтово
    
    0. Выход\n
    """
    option = -1

    while (option != 0):
        print(menuText)

        try:
            option = int(input("Выберите пункт меню: "))
        except:
            print("\nОшибка: неверный пункт меню")
            continue

        if (option == 1):
            string = input("Введите строку (только Латинский алфавит и символы \"?!. \"): ").upper()
            print()
            processString(string)
        elif (option == 2):
            filePath = input("Введите путь до файла (по умолчанию {0}): "
                .format(config.defaultTestProgram))

            if (filePath == ''):
                processBinary()
            else:
                processBinary(filePath)
        elif (option == 0):
            break
    

if __name__ == "__main__":
    main()
