#ifndef _IO_HPP_
#define _IO_HPP_

#include <iterator>

#include "../huffman/huffman_defines.hpp"

BytesSequence read_binary_file(std::string &in_file_name);
std::tuple<HuffmanFrequencyTable, int, BytesSequence>
    read_compressed_file(std::string &in_file_name);

void write_binary_file(std::string &out_file_name,
                       std::tuple<HuffmanFrequencyTable, int, BytesSequence> &out_buf);

void write_decompressed_file(std::string &out_file_name, BytesSequence &byte_seq);

#endif
