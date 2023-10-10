#ifndef _HUFFMAN_HPP_
#define _HUFFMAN_HPP_

#include "../io/io.hpp"

int huffman_compress_file(std::string &in_file_name, std::string &out_file_name);
int huffman_decompress_file(std::string &in_file_name, std::string &out_file_name);

#endif
