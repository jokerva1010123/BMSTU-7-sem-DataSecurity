import random
import itertools

import logging
logging.basicConfig(format='%(message)s', level=logging.INFO)

class Reflector():
    def __init__(self, pairs):
        self._pairs = dict()
        if isinstance(pairs, int):
            tmp = [x for x in range(pairs)]
            random.shuffle(tmp)

            for i in range(0, len(tmp), 2):
                self._pairs[tmp[i]] = tmp[i + 1]
                self._pairs[tmp[i + 1]] = tmp[i]
        else:
            self._pairs = pairs

        logging.debug("\nPairs:\n{0}".format(self._pairs))


    def SetConfig(self, pairs):
        self._pairs = pairs


    def GetConfig(self):
        return self._pairs


    def Reflect(self, key):
        return self._pairs[key]


if __name__ == "__main__":
    ref = Reflector(26)
    print(ref.Reflect(3))
    print(ref.Reflect(ref.Reflect(3)))
