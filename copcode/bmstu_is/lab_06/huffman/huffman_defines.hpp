#ifndef _HUFFMAN_DEFINES_HPP_
#define _HUFFMAN_DEFINES_HPP_

#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>

using frequencyType = uint32_t;
using BytesSequence = std::vector<uint8_t>;
using HuffmanFrequencyTable = std::map<uint8_t, frequencyType>;
using BitsSequence = std::vector<bool>;
using HuffmanMap = std::unordered_map<uint8_t, BitsSequence>;

#endif
