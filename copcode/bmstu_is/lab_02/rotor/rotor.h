#ifndef _ROTOR_H_
#define _ROTOR_H_

#include <array>
#include <memory>
#include "../constants.h"

#define COUNT_ROTORS 3
#define FAST_ROTOR_CONFIG_FILE "config_data/fast_rotor.txt"
#define MIDDLE_ROTOR_CONFIG_FILE "config_data/middle_rotor.txt"
#define SLOW_ROTOR_CONFIG_FILE "config_data/slow_rotor.txt"

static std::string rotors[3] = {FAST_ROTOR_CONFIG_FILE,
             MIDDLE_ROTOR_CONFIG_FILE, 
             SLOW_ROTOR_CONFIG_FILE};

class Rotor
{
    private:
        int __offset = 0; // rotation symbol offset
        bool __full_cycle = false;
        std::array<int, COUNT_SYMBOLS> __original_seq, __shuffled_seq;

        void __reverse(int i, int j);

    public:
        Rotor() = default;
        Rotor(std::string file_name);
        explicit Rotor(const Rotor &rotor);

        ~Rotor() = default;

        void reset();
        int forward(int index);
        int backward(int index);
        
        void rotate();
        bool is_full_cycle();
        
        void output_orig_seq();
        void output_shuffled_seq();

        void output_values();

        void set_values(std::array<int, COUNT_SYMBOLS> &rotor);
};

#endif