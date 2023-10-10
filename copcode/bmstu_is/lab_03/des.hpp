#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <cstdint>

constexpr int BLOCK_SIZE = 64;
constexpr int BITS_IN_BYTE = 8;
constexpr int FILE_SIZE_MULTIPLICITY = BLOCK_SIZE / BITS_IN_BYTE;

std::vector<uint8_t> DES(uint8_t mode, const std::vector<uint8_t>& from, uint64_t key64b);
