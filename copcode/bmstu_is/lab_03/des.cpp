#include "des.hpp"

#include <iostream>
#include <span>

#define LSHIFT_28BIT(x, L) ((((x) << (L)) | ((x) >> (-(L) & 27))) & (((uint64_t)1 << 32) - 1))

static constexpr std::array<std::array<std::array<uint8_t, 16>, 4>, 8> Sbox = {{
	{{ // 0
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0, 7 },
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3, 8 },
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5, 0 },
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
	}},
	{{ // 1
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11, 5 },
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14, 9 },
	}},
	{{ // 2
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2, 8 },
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15, 1 },
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14, 7 },
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
	}},
	{{ // 3
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14, 9 },
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8, 4 },
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
	}},
	{{ // 4
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14, 9 },
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8, 6 },
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5, 3 },
	}},
	{{ // 5
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3, 8 },
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11, 6 },
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
	}},
	{{ // 6
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6, 1 },
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8, 6 },
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9, 2 },
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
	}},
	{{ // 7
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12, 7 },
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9, 2 },
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5, 8 },
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
	}},
}};

static constexpr std::array<uint8_t, 64> IP = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
};

static constexpr std::array<uint8_t, 64> FP = {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25,
};

static constexpr std::array<uint8_t, 28> K1P = {
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
};

static constexpr std::array<uint8_t, 28> K2P = {
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4,
};

static constexpr std::array<uint8_t, 48> CP = {
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32,
};

static constexpr std::array<uint8_t, 48> EP = {
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1,
};

static constexpr std::array<uint8_t, 32> P = {
	16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25,
};

static uint64_t join_8x8bits_to_64bits(const std::span<const uint8_t>& blocks8x8b) {
	uint64_t block64b;
	for (auto block8b : blocks8x8b) {
		block64b = (block64b << 8u) | block8b;
	}
	return block64b;
}

static uint64_t join_2x32bits_to_64bits(const std::array<uint32_t, 2>& blocks2x32b) {
	auto block64b = static_cast<uint64_t>(blocks2x32b[0]);
	block64b = (block64b << 32u) | blocks2x32b[1];
	return block64b;
}

static uint64_t join_2x28bits_to_56bits(const std::array<uint32_t, 2> blocks2x28b) {
	auto block56b = static_cast<uint64_t>(blocks2x28b[0] >> 4u);
	block56b = ((block56b << 32u) | blocks2x28b[1]) << 4u;
	return block56b;
}

static uint32_t join_8x4bits_packed_to_32bits(const std::array<uint8_t, 4>& blocks8x4b_packed) {
	uint32_t block32b;
	for (auto block8b : blocks8x4b_packed) {
		block32b = (block32b << 8u) | block8b;
	}
	return block32b;
}

static void split_64bits_to_8x8bits(uint64_t block64b, const std::span<uint8_t>& blocks8x8b) {
	for (uint8_t i = 0; i < 8; ++i) {
		blocks8x8b[i] = block64b >> ((7u - i) * 8u);
	}
}

static std::array<uint32_t, 2> split_64bits_to_2x32bits(uint64_t block64b) {
	return {static_cast<uint32_t>(block64b >> 32u), static_cast<uint32_t>(block64b)};
}

static std::array<uint8_t, 8> split_48bits_to_8x6bits(uint64_t block48b) {
	std::array<uint8_t, 8> blocks8x6b{};
	for (uint8_t i = 0; i < 8; ++i) {
		blocks8x6b[i] = (block48b >> (58u - (i * 6u))) << 2u;
	}
	return blocks8x6b;
}

static uint8_t extreme_bits(uint8_t block6b) {
	return ((block6b >> 6u) & 0x2u) | ((block6b >> 2u) & 0x1u);
}

static uint8_t middle_bits(uint8_t block6b) {
	return (block6b >> 3u) & 0xFu;
}

static std::array<uint8_t, 4> substitution_8x6bits_to_8x4bits_packed(const std::array<uint8_t, 8>& blocks6b) {
	std::array<uint8_t, 4> blocks4b_packed{};
	for (uint8_t i = 0, j = 0; i < 8; i += 2, ++j) {
		uint8_t block2b = extreme_bits(blocks6b[i]);
		uint8_t block4b = middle_bits(blocks6b[i]);
		blocks4b_packed[j] = Sbox[i][block2b][block4b];

		block2b = extreme_bits(blocks6b[i + 1]);
		block4b = middle_bits(blocks6b[i + 1]);
		blocks4b_packed[j] = (blocks4b_packed[j] << 4u) | Sbox[i + 1][block2b][block4b];
	}
	return blocks4b_packed;
}

static uint64_t expansion_permutation(uint32_t block32b) {
	uint64_t block48b = 0;
	for (uint8_t i = 0; i < 48; ++i) {
		block48b |= (uint64_t)((block32b >> (32u - EP[i])) & 0x01u) << (63u - i);
	}
	return block48b;
}

static uint32_t substitutions(uint64_t block48b) {
	const auto blocks8x6b = split_48bits_to_8x6bits(block48b);
	const auto blocks8x4b_packed = substitution_8x6bits_to_8x4bits_packed(blocks8x6b);
	return join_8x4bits_packed_to_32bits(blocks8x4b_packed);
}

static uint32_t permutation(uint32_t block32b) {
	uint32_t new_block32b = 0;
	for (uint8_t i = 0; i < 32; ++i) {
		new_block32b |= ((block32b >> (32u - P[i])) & 0x01u) << (31u - i);
	}
	return new_block32b;
}

static uint32_t func_F(uint32_t block32b, uint64_t key48b) {
	uint64_t block48b = expansion_permutation(block32b);
	block48b ^= key48b;
	block32b = substitutions(block48b);
	return permutation(block32b);
}

static void round_feistel_cipher(uint32_t& N1, uint32_t& N2, uint64_t key48b) {
	uint32_t tmp = N2;
	N2 = func_F(N2, key48b) ^ N1;
	N1 = tmp;
}

static void feistel_cipher(uint8_t mode, uint32_t& N1, uint32_t& N2, const std::array<uint64_t, 16>& keys16x48b) {
	switch(mode) {
	case 'E':
	case 'e':
		for (int8_t round = 0; round < 16; ++round) {
			round_feistel_cipher(N1, N2, keys16x48b[round]);
		}
		std::swap(N1, N2);
		break;
	case 'D':
	case 'd':
		for (int8_t round = 15; round >= 0; --round) {
			round_feistel_cipher(N1, N2, keys16x48b[round]);
		}
		std::swap(N1, N2);
		break;
	default:
		std::cerr << "Unknown mode: " << mode << "\n";
		break;
	}
}

static uint64_t initial_permutation(uint64_t block64b) {
	uint64_t new_block64b = 0;
	for (uint8_t i = 0; i < 64; ++i) {
		new_block64b |= ((block64b >> (64u - IP[i])) & 0x01u) << (63u - i);
	}
	return new_block64b;
}

static uint64_t final_permutation(uint64_t block64b) {
	uint64_t new_block64b = 0;
	for (uint8_t i = 0; i < 64; ++i) {
		new_block64b |= ((block64b >> (64u - FP[i])) & 0x01u) << (63u - i);
	}
	return new_block64b;
}

static uint64_t key_contraction_permutation(uint64_t block56b) {
	uint64_t block48b = 0;
	for (uint8_t i = 0; i < 48; ++i) {
		block48b |= ((block56b >> (64u - CP[i])) & 0x01u) << (63u - i);
	}
	return block48b;
}

static std::array<uint32_t, 2> key_permutation_56bits_to_2x28bits(uint64_t key56b) {
	std::array<uint32_t, 2> keys2x28b{};
	for (uint8_t i = 0; i < 28u; ++i) {
		keys2x28b[0] |= ((key56b >> (64u - K1P[i])) & 0x01u) << (31u - i);
		keys2x28b[1] |= ((key56b >> (64u - K2P[i])) & 0x01u) << (31u - i);
	}
	return keys2x28b;
}

static std::array<uint64_t, 16> key_expansion_2x28bits_to_16x48bits(std::array<uint32_t, 2> keys2x28b) {
	std::array<uint64_t, 16> keys16x48b{};
	for (uint8_t i = 0; i < 16; ++i) {
		uint8_t n;
		switch(i) {
			case 0: case 1: case 8: case 15: n = 1; break;
			default: n = 2; break;
		}

		keys2x28b[0] = LSHIFT_28BIT(keys2x28b[0], n);
		keys2x28b[1] = LSHIFT_28BIT(keys2x28b[1], n);
		const uint64_t block56b = join_2x28bits_to_56bits(keys2x28b);
		keys16x48b[i] = key_contraction_permutation(block56b);
	}
	return keys16x48b;
}

static std::array<uint64_t, 16> key_expansion(uint64_t key64b) {
	auto keys2x28b = key_permutation_56bits_to_2x28bits(key64b);
	return key_expansion_2x28bits_to_16x48bits(keys2x28b);
}

std::vector<uint8_t> DES(uint8_t mode, const std::vector<uint8_t>& from, uint64_t key64b) {
	assert(from.size() % FILE_SIZE_MULTIPLICITY == 0);

	std::vector<uint8_t> to(from.size());

	auto keys16x48b = key_expansion(key64b);

	for (size_t i = 0; i < from.size(); i += 8) {
		auto keys2x32b = split_64bits_to_2x32bits(
			initial_permutation(
				join_8x8bits_to_64bits(std::span<const uint8_t>(&from[i], 8))
			)
		);
		feistel_cipher(mode, keys2x32b[0], keys2x32b[1], keys16x48b);
		split_64bits_to_8x8bits(
			final_permutation(
				join_2x32bits_to_64bits(keys2x32b)
			),
			std::span(&to[i], 8)
		);
	}
	return to;
}
