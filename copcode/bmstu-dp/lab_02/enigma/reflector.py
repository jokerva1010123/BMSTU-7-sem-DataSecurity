from random import choice
from constants import Settings


class Reflector:
    def __init__(self, alphabet, filling_type):
        self.dictionary = {}

        if filling_type == Settings.random_filling:
            self.__get_dictionary(alphabet)
        elif filling_type == Settings.file_filling:
            self.__get_dictionary_file(alphabet)

    def get_letter(self, letter):
        return self.dictionary.get(letter)

    def save_settings(self, path):
        with open(path, 'w') as file:
            for pair in self.dictionary.items():
                file.write(str(pair[0]) + ' ' + str(pair[1]) + '\n')

    def __get_dictionary(self, alphabet_size):
        alphabet = [letter for letter in range(alphabet_size)]

        while alphabet:
            first = choice(alphabet)
            alphabet.remove(first)

            second = choice(alphabet)
            alphabet.remove(second)

            self.dictionary[first] = second
            self.dictionary[second] = first

    def __get_dictionary_file(self, path):
        with open(path, 'r') as file:
            for pair in file:
                pair = pair.split()
                self.dictionary[int(pair[0])] = int(pair[1])
