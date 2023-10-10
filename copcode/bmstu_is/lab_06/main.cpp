#include <iostream>
#include "./io/io.hpp"
#include "./huffman/huffman.hpp"

int usage()
{
    std::cout << "Неверное число параметров. Запуск: <app> <c|d> <in file name> <out file name>.\n";
    return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    std::string in_file_name {}, out_file_name {}, mode {};

    if (argc < 4){
        return usage();
    }

    mode = argv[1]; //mode: compress or decompress
    in_file_name = argv[2]; //input | output file name
    out_file_name = argv[3];

    if (mode == "c"){
        huffman_compress_file(in_file_name, out_file_name);
    }
    else if (mode == "d"){
        huffman_decompress_file(in_file_name, out_file_name);
    }

    return EXIT_SUCCESS;
}
