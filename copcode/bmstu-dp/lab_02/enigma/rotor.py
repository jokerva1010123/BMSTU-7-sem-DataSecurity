from random import shuffle, randint
from constants import Settings


class Rotor:
    def __init__(self, alphabet, filling_type):
        if filling_type == Settings.random_filling:
            self.alphabet = [letter for letter in range(alphabet)]
            shuffle(self.alphabet)
            self.start_state = randint(0, alphabet - 1)
            self.state = self.start_state
        elif filling_type == Settings.file_filling:
            self.__get_from_file(alphabet)

        self.clicks = 0

    def encipher_letter(self, letter, direction):
        if direction == Settings.direct:
            result = self.alphabet[(letter + self.state) % len(self.alphabet)]
        elif direction == Settings.reverse:
            result = (self.alphabet.index(letter) - self.state) % len(self.alphabet)

        return result

    def rotate(self):
        self.state += 1
        self.clicks += 1

    def save_settings(self, path):
        with open(path, 'w') as file:
            file.write(str(self.start_state) + '\n')

            for letter in self.alphabet:
                file.write(str(letter) + '\n')

    def __get_from_file(self, path):
        with open(path, 'r') as file:
            flag = True

            self.alphabet = []

            for line in file:
                if flag:
                    self.start_state = int(line)
                    self.state = int(line)
                    flag = False
                else:
                    self.alphabet.append(int(line))
