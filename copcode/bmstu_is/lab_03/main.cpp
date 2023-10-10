#include <fstream>
#include <iostream>

#include "des.hpp"

int main(int argc, char** argv) {
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <mode: e|d> <input file> <output file>\n";
		return EXIT_FAILURE;
	}

	if (const std::string mode(argv[1]); !(mode == "e" || mode == "d")) {
		std::cerr << "Invalid mode: " << mode << "\n";
		return EXIT_FAILURE;
	}

	std::ifstream input(argv[2], std::ios::in | std::ios::binary);
	if (!input) {
		std::cerr << argv[2] << ": can't open file\n";
		return EXIT_FAILURE;
	}

	std::vector<uint8_t> from(
		(std::istreambuf_iterator<char>(input)),
		(std::istreambuf_iterator<char>()));
	input.close();

	if (const int rem = from.size() % FILE_SIZE_MULTIPLICITY; rem != 0) {
		std::cerr << "Warning: file size is not a multiple of the block: " << from.size() << " % " << FILE_SIZE_MULTIPLICITY << " = " << rem << "\n";
		from.resize(from.size() + FILE_SIZE_MULTIPLICITY - rem);
	}

	constexpr uint64_t key64b = 0xABADBABEDEADBABE;

	auto to = DES(argv[1][0], from, key64b);

	std::ofstream output(argv[3], std::ios::out | std::ios::binary);
	if (!output) {
		std::cerr << argv[3] << ": can't open file\n";
		return EXIT_FAILURE;
	}
    std::cout << "to size: " << to.size();

	output.write(reinterpret_cast<const char*>(&to[0]), to.size());
	output.close();
}
