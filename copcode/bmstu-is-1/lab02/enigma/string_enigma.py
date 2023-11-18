class StringEnigma:

    def __init__(self, enigma):
        self._enigma = enigma


    def SetEnigma(self, enigma):
        self._enigma = enigma


    def GetEnigmaConfig(self):
        return self._enigma.GetConfig()


    def Reset(self):
        self._enigma.Reset()


    def Encipher(self, string):
        result = ""
        for char in string:
            result += self._enigma.Encipher(char)

        return result
