#ifndef _IO_H_
#define _IO_H_

#include <iostream>
#include <memory>

#include "../constants.h"

#define MAX_FILE_SIZE 10240

bool read_bin_file(std::unique_ptr<unsigned char []> &plain_buffer, int &file_size, std::string file_name);
bool write_bin_file(std::unique_ptr<unsigned char []> &cipher_buffer, int &buffer_size, std::string file_name);
bool read_txt_file(std::array<int, COUNT_SYMBOLS> &plug_buffer, int file_size, std::string file_name);

#endif