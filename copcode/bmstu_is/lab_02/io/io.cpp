#include <fstream>
#include <iostream>
#include "io.h"

bool read_bin_file(std::unique_ptr<unsigned char []> &plain_buffer, int &file_size, std::string file_name)
{
    int success = true, buffer_size;
    std::ifstream file;

    if (file_name.empty()){
        std::cout << "Указано пустое имя файла.\n";
        return false;
    }

    file.open(file_name, std::ios::in | std::ios::binary);

    if (file.fail()){
        std::cout << "Ошибка открытия файла на чтение" << file_name << "\n";
        return false;
    }

    // define file size
    file.seekg(0, std::ios::end);
    file_size = file.tellg();

    if (file_size == 0){
        std::cout << "Файл пуст \n";
        success = false;
    }
    else{
        file.seekg(0, std::ios::beg);

        if (file_size > MAX_FILE_SIZE){
            file_size = MAX_FILE_SIZE;
        }

        // read data from binary file
        plain_buffer = std::make_unique<unsigned char []>(file_size);
        file.read((char *)plain_buffer.get(), file_size);
    }
 
    file.close();

    return success;
}

bool write_bin_file(std::unique_ptr<unsigned char []> &cipher_buffer, int &buffer_size, std::string file_name){
    int success = true;
    std::ofstream file;

    file.open(file_name, std::ios::out | std::ios::binary);

    if (!file.is_open()){
        std::cout << "Ошибка открытия файла на запись " << file_name << "\n";
        success = false;
    }
    else{
        auto buffer_data = cipher_buffer.get();
        file.write(reinterpret_cast<const char *>(buffer_data), buffer_size);
            
        if (!file.good()){
            std::cout << "Ошибка записи данных в файл" << file_name << "\n";
            success = false;
        }
    }

    file.close();

    return success;
}

bool read_txt_file(std::array<int, COUNT_SYMBOLS> &plug_buffer, int file_size, std::string file_name)
{
    int success = true, buffer_size;
    std::ifstream file;

    if (file_name.empty()){
        std::cout << "Указано пустое имя файла.\n";
        return false;
    }

    file.open(file_name, std::ios::in);

    if (file.fail()){
        std::cout << "Ошибка открытия файла на чтение" << file_name << "\n";
        return false;
    }

    // define file size
    file.seekg(0, std::ios::end);
    file_size = file.tellg();

    if (file_size == 0){
        std::cout << "Файл пуст \n";
        success = false;
    }
    else{
        file.seekg(0, std::ios::beg);

        // read data from txt file
        //std::cout << "\nplug_file: \n";
        for (int i = 0; i < COUNT_SYMBOLS && success == true; i++){
            file >> plug_buffer[i];
            if (file.fail()){
                success = false;
            }
            //std::cout << plug_buffer[i] << " ";
        }
    }

    file.close();
    return success;
}