#include <iostream>
#include <cstdlib>

#include "./io/io.h"
#include "./enigma/enigma.h"

int main(int argc, char *argv[])
{
    if (argc != 2){
        std::cout << "Ошибка: необходимо указать 2 аргумента (имя программы + имя файла)";
        return EXIT_FAILURE;
    }

    int file_size;
    std::unique_ptr<unsigned char []> plain_buffer;
    if (!read_bin_file(plain_buffer, file_size, argv[1])){
        std::cout << "Ошибка чтения файла " << argv[1] << ".\n";
        return EXIT_FAILURE;
    }

    Enigma enigma;
    std::unique_ptr<unsigned char []> cipher_buffer, origin_buffer;

    //enciphering
    cipher_buffer = enigma.encipher(plain_buffer, file_size);

    enigma.reset();
    //deciphering
    origin_buffer = enigma.decipher(cipher_buffer, file_size);

    if (!write_bin_file(cipher_buffer, file_size, CYPHER_FILE)){
        return EXIT_FAILURE;
    }

    if (!write_bin_file(origin_buffer, file_size, ORIGIN_FILE)){
        return EXIT_FAILURE;
    }
    
    std::cout << "Шифрование и дешифровка выполнены успешно.\n";
    std::cout << "Зашифрованный файл: " << CYPHER_FILE << "\n"; 
    std::cout << "Расшифрованный файл: " << ORIGIN_FILE << "\n";

    return EXIT_SUCCESS;
}