#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include <memory>
#include "../rotor/rotor.h"
#include "../reflector/reflector.h"

class Enigma
{
    private:
        int __seed;
        Rotor __rotors[COUNT_ROTORS]; // 0 - fast, 1 - middle, 2 - slow
        Reflector __reflector;

        int __encipher_symbol(int plain_symbol);
    public:
        Enigma();

        ~Enigma() = default;
        std::unique_ptr<unsigned char []> encipher(std::unique_ptr<unsigned char []> &plain_buffer, int &buffer);
        std::unique_ptr<unsigned char []> decipher(std::unique_ptr<unsigned char []> &plain_buffer, int &buffer_size);
        void reset();
        void output_rotor_values();
        void output_reflector_values();
};

#endif