#include "io.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

uintmax_t file_size(const char* filename) {
    std::filesystem::path p{filename};
    return std::filesystem::file_size(p);
}

mpz_class read_mpz(const char* filename) {
    FILE* input = fopen(filename, "rb");
    if (!input) {
        std::cerr << filename << ": can't open file\n";
        throw;
    }

    const uintmax_t bytes = file_size(filename);
    const uintmax_t bits = bytes * 8;

    mpz_t n;
    mpz_init2(n, bits);

    fread((void *) n->_mp_d, bytes, 1, input);

    fclose(input);

    for (int i = n->_mp_alloc - 1; i >= 0; --i) {
        if (n->_mp_d[i] != 0) {
            n->_mp_size = i + 1;
            break;
        }
    }

    mpz_class nc{n};
    mpz_clear(n);
    return nc;
}

void write_mpz(const char* filename, const mpz_class n) {
    FILE* output = fopen(filename, "wb");
    if (!output) {
        std::cerr << filename << ": can't open file\n";
        throw;
    }

    int size = mpz_sizeinbase(n.get_mpz_t(), 256);

    fwrite((void *) n.get_mpz_t()->_mp_d, size, 1, output);

    fclose(output);
}

std::pair<mpz_class, mpz_class> read_key(const char* filename) {
    FILE* input = fopen(filename, "rb");
    if (!input) {
        std::cerr << filename << ": can't open file\n";
        throw;
    }

    mpz_class e;
    mpz_class n;
    mpz_inp_str(e.get_mpz_t(), input, 10);
    mpz_inp_str(n.get_mpz_t(), input, 10);

    fclose(input);

    return {e, n};
}

void write_key(const char* filename, const mpz_class& e, const mpz_class& n) {
    FILE* output = fopen(filename, "wb");
    if (!output) {
        std::cerr << filename << ": can't open file\n";
        throw;
    }

    const auto write = [=](mpz_class num) {
        mpz_out_str(output, 10, num.get_mpz_t());
        putc('\n', output);
    };

    write(e);
    write(n);

    fclose(output);
}
