#include <chrono>
#include <iostream>
#include "enigma.h"

Enigma::Enigma()
{
    //init rotors and reflector
    for (int i = 0; i < COUNT_ROTORS; i++){
        __rotors[i] = Rotor(rotors[i]);
    }

    __reflector = Reflector(REFLECTOR_CONFIG_FILE);
}

std::unique_ptr<unsigned char []> Enigma::encipher(std::unique_ptr<unsigned char []> &plain_buffer, int &buffer_size){
    std::unique_ptr<int []> cipher_int_buffer = std::make_unique<int []>(buffer_size);
    std::unique_ptr<unsigned char []> cipher_char_buffer = std::make_unique<unsigned char []>(buffer_size);
    
    //ecrypt each byte
    for (int i = 0; i < buffer_size; i++){
        cipher_int_buffer[i] = __encipher_symbol(plain_buffer[i]);
        cipher_char_buffer[i] = static_cast<unsigned char>(cipher_int_buffer[i]);
    }

    return cipher_char_buffer;
}

int Enigma::__encipher_symbol(int plain_symbol){
    int encrypted = plain_symbol;
    for (int j = 0; j < COUNT_ROTORS; j++){
        encrypted = __rotors[j].forward(encrypted);
    }

    encrypted = __reflector.forward(encrypted);

    for (int j = COUNT_ROTORS - 1; j >= 0; j--){
        encrypted = __rotors[j].backward(encrypted);
    }

    __rotors[0].rotate();

    for (int j = 1; j < COUNT_ROTORS; j++){
        if (__rotors[j - 1].is_full_cycle()){
            __rotors[j].rotate();
        }
    }

    return encrypted;
}

void Enigma::reset(){
    for (int i = 0; i < COUNT_ROTORS; i++){
        __rotors[i].reset();
    }
}

void Enigma::output_rotor_values(){
    for (int i = 0; i < COUNT_ROTORS; i++){
        __rotors[i].output_values();
    }
}

void Enigma::output_reflector_values(){
    __reflector.output_reflector_values();
}

std::unique_ptr<unsigned char []> Enigma::decipher(std::unique_ptr<unsigned char []> &plain_buffer, int &buffer_size){
    return this->encipher(plain_buffer, buffer_size);
}
