#pragma once

#include <array>
#include <inttypes.h>
#include <string>

using namespace std;

constexpr uint16_t ASCII_SIZE = 256;

using FreqPair = pair<string, uint64_t>; // symb freq
using FreqTable = array<FreqPair, ASCII_SIZE>;
