class TextEnigma:

    def __init__(self, enigma):
        self._enigma = enigma


    def SetEnigma(self, enigma):
        self._enigma = enigma


    def GetEnigmaConfig(self):
        return self._enigma.GetConfig()


    def Reset(self):
        self._enigma.Reset()


    def Encipher(self, inFilename, outFilename="encodedText.txt"):

        with (open(inFilename, "r") as inFile,
              open(outFilename, "w") as outFile):

            for line in inFile:
                for char in line:
                    outFile.write(self._enigma.Encipher(char))
