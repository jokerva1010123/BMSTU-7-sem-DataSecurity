#include <iostream>
#include "rotor.h"
#include "../io/io.h"
#include "../constants.h"

Rotor::Rotor(std::string file_name){
    std::array<int, COUNT_SYMBOLS> rotor;
    read_txt_file(rotor, COUNT_SYMBOLS, file_name);

    __shuffled_seq = std::move(rotor);
    /*// init original sequence
    for (int i = 0; i < COUNT_SYMBOLS; i++){
        __original_seq[i] = i;
    }

    // shuffled sequence
    __shuffled_seq = __original_seq;
    std::shuffle(__shuffled_seq.begin(), __shuffled_seq.begin() + COUNT_SYMBOLS, 
                std::default_random_engine(seed));*/
}

void Rotor::output_orig_seq(){
    std::cout << "Orignal rotor: ";
    for (int seq_elem: __original_seq){
        std::cout << seq_elem << ' ';
    }
    std::cout << "\n";
}

void Rotor::output_shuffled_seq(){
    std::cout << "Shuffled rotor: ";
    for (int seq_elem: __shuffled_seq){
        std::cout << seq_elem << ' ';
    }
    std::cout << "\n";
}

void Rotor::set_values(std::array<int, COUNT_SYMBOLS> &rotor){
    __shuffled_seq = std::move(rotor);
}

void Rotor::__reverse(int i, int j){
    while (i < j){
        std::swap(__shuffled_seq[i], __shuffled_seq[j]);
        i++, j--;
    }
}

void Rotor::reset(){
    // right logical shift
    int right_offset = COUNT_SYMBOLS - __offset;
    __reverse(0, COUNT_SYMBOLS - 1);
    __reverse(0, right_offset - 1);
    __reverse(right_offset, COUNT_SYMBOLS - 1);

    __offset = 0;
}

int Rotor::forward(int index){
    return __shuffled_seq[index];
}

int Rotor::backward(int value){
    bool is_found = false;
    int index;

    for (int i = 0; i < COUNT_SYMBOLS && is_found == false; ++i){
        if (__shuffled_seq[i] == value){
            index = i;
            is_found = true;
        }
    }

    return index;
}

void Rotor::rotate(){
    int last_elem = __shuffled_seq[COUNT_SYMBOLS - 1];
    for (int i = COUNT_SYMBOLS - 1 ; i > 0; --i){
        __shuffled_seq[i] = __shuffled_seq[i-1];
    }
    __shuffled_seq[0] = last_elem;

    __offset += 1;
    if (__offset == COUNT_SYMBOLS){
        __offset = 0;
        __full_cycle = true;
    }
    else{
        __full_cycle = false;
    }
}

bool Rotor::is_full_cycle(){
    return __full_cycle;
}

void Rotor::output_values()
{
    std::cout << "Rotor values:\n";
    for (auto it = __shuffled_seq.begin(); it != __shuffled_seq.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << "\n";
}