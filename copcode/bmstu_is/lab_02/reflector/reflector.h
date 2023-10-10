#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include <array>
#include <iostream>
#include "../constants.h"

#define REFLECTOR_CONFIG_FILE "config_data/reflector.txt" 

class Reflector
{
    private:
        std::array<int, COUNT_SYMBOLS> __original_seq, __shuffled_seq;
    public:
        Reflector() = default;
        Reflector(std::string file_name);
        ~Reflector() = default;

        int forward(int index);
        void output_reflector_values();
        void set_values(std::array<int, COUNT_SYMBOLS> &reflector);
};

#endif