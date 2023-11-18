from enigma.rotor import Rotor
from enigma.reflector import Reflector

from string import ascii_letters

class Enigma:
    def __init__(self, alphabet, rotorsNumber=3):
        self._alphabet = alphabet
        self._rotors = [Rotor(len(alphabet)) for _ in range(rotorsNumber)]
        self._reflector = Reflector(len(alphabet))


    def SetConfig(self, rotorsConfigs, reflectorConfig):
        for i, rotor in enumerate(self._rotors):
            rotor.SetConfig(*rotorsConfigs[i])

        self._reflector.SetConfig(reflectorConfig)


    def GetConfig(self):
        rotorsConfigs = [rotor.GetConfig() for rotor in self._rotors]

        return rotorsConfigs, self._reflector.GetConfig()


    def _rotate(self):
        notFull = True

        i = 0
        while notFull and i < len(self._rotors):
            notFull = self._rotors[i].Rotate()
            i += 1


    def Reset(self):
        self.SetConfig(*self.GetConfig())


    def Encipher(self, char):
        charCode = self._alphabet.index(char)
        #print(self._alphabet[charCode])

        for rotor in self._rotors:
            charCode = rotor.GetForward(charCode)
            #print(self._alphabet[charCode])

        charCode = self._reflector.Reflect(charCode)
        #print(self._alphabet[charCode])

        for rotor in reversed(self._rotors):
            charCode = rotor.GetInverse(charCode)
            #print(self._alphabet[charCode])

        self._rotate()

        return self._alphabet[charCode]


if __name__ == "__main__":
    enigma = Enigma(ascii_letters)

    print("ENIGMA CONF")
    print(enigma.GetConfig())
    print(enigma._alphabet)

    print("TEST")
    benchmark = "itworksyay"
    print("ORIGINAL", benchmark)
    encryptedMsg = ""
    for char in benchmark:
        encryptedMsg += enigma.Encipher(char)

    print("ENCRYPTED", encryptedMsg)
    enigma.SetConfig(*enigma.GetConfig())

    originalMsg = ""
    for char in encryptedMsg:
        originalMsg += enigma.Encipher(char)

    print("ORIGINAL", originalMsg)

