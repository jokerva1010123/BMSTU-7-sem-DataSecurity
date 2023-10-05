import random
import sys

import logging
logging.basicConfig(format='%(message)s', level=logging.INFO)

class Rotor:

    def __init__(self, alphabet, beginPosition=None):

        if isinstance(alphabet, int):
            self._alphabet = [x for x in range(alphabet)]
            random.shuffle(self._alphabet)
        else:
            self._alphabet = alphabet.copy()

        self._position = (beginPosition if beginPosition is not None
                            else random.randint(0, len(self._alphabet)))
        self._initPos = self._position

        logging.debug("\nAlphabet:\n{0}".format(self._alphabet))
        logging.debug("\nPosition:\n{0}".format(self._position))


    def _setAlphabet(self, alphabet):
        self._alphabet = alphabet.copy()


    def _setPosition(self, position):
        self._position = position
        self._initPos = self._position


    def SetConfig(self, alphabet, position):
        self._setAlphabet(alphabet)
        self._setPosition(position)

        logging.debug("\nAlphabet:\n{0}".format(self._alphabet))
        logging.debug("\nPosition:\n{0}".format(self._position))


    def GetConfig(self):
        return self._alphabet, self._initPos


    def GetForward(self, index):
        return self._alphabet[(index + self._position) % len(self._alphabet)]


    def GetInverse(self, value):
        return (self._alphabet.index(value)
                    - self._position) % len(self._alphabet)


    def Rotate(self):
        self._position += 1
        return self._position % len(self._alphabet)


    def Reset(self):
        self._position = self._initPos


if __name__ == "__main__":
    rotor = Rotor(26)

    rotor1 = Rotor(26)

    rotorCopy1 = Rotor(*rotor.GetConfig())

    rotor1.SetConfig(*rotor.GetConfig())

    alphabet = list("abcde")

    test = Rotor(list(reversed([x for x in range(len(alphabet))])), 0)

    for i in range(len(alphabet)):
        logging.debug("\nforward: {0}".format(alphabet[test.GetForward(i)]))
        logging.debug("\ninverse: {0}".format(alphabet[test.GetInverse(i)]))

