from enigma.rotor import Rotor
from enigma.reflector import Reflector
from constants import Settings


class Enigma:
    def __init__(self, alphabet, filling_type):
        self.alphabet = alphabet

        if filling_type == Settings.random_filling:
            self.fast_rotor = Rotor(len(alphabet), filling_type)
            self.middle_rotor = Rotor(len(alphabet), filling_type)
            self.slow_rotor = Rotor(len(alphabet), filling_type)

            self.reflector = Reflector(len(alphabet), filling_type)
        elif filling_type == Settings.file_filling:
            self.fast_rotor = Rotor('settings/fast_rotor.txt', filling_type)
            self.middle_rotor = Rotor('settings/middle_rotor.txt', filling_type)
            self.slow_rotor = Rotor('settings/slow_rotor.txt', filling_type)

            self.reflector = Reflector('settings/reflector.txt', filling_type)      

    def encipher(self, letter):
        letter_code = self.__get_letter_code(letter)

        letter_code = self.fast_rotor.encipher_letter(letter_code, Settings.direct)
        letter_code = self.middle_rotor.encipher_letter(letter_code, Settings.direct)
        letter_code = self.slow_rotor.encipher_letter(letter_code, Settings.direct)

        letter_code = self.reflector.get_letter(letter_code)

        letter_code = self.slow_rotor.encipher_letter(letter_code, Settings.reverse)
        letter_code = self.middle_rotor.encipher_letter(letter_code, Settings.reverse)
        letter_code = self.fast_rotor.encipher_letter(letter_code, Settings.reverse)

        self.__rotate()

        return self.alphabet[letter_code]

    def save_settings(self):
        self.fast_rotor.save_settings('settings/fast_rotor.txt')
        self.middle_rotor.save_settings('settings/middle_rotor.txt')
        self.slow_rotor.save_settings('settings/slow_rotor.txt')

        self.reflector.save_settings('settings/reflector.txt')

    def __rotate(self):
        self.fast_rotor.rotate()

        if self.fast_rotor.clicks % len(self.alphabet) == 0:
            self.middle_rotor.rotate()

        if self.middle_rotor.clicks % len(self.alphabet) == 0 and self.fast_rotor.clicks > 0 and self.middle_rotor.clicks > len(self.alphabet) - 1:
            self.slow_rotor.rotate()

    def __get_letter_code(self, letter):
        return self.alphabet.index(letter)
