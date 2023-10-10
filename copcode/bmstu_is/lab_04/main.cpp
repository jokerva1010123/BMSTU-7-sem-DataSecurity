#include <cassert>
#include <filesystem>
#include <iostream>
#include <map>
#include "io/io.hpp"
#include "key_gen/key_gen.hpp"
#include "rsa/rsa.hpp"

int usage(const char* argv0) {
    std::cerr << "Usage for keygen: " << argv0 << " keygen [<p> <q> [e]]\n";
    std::cerr << "Usage for crypt:  " << argv0 << " <mode: e|d> <input file> <output file>\n";
    return EXIT_FAILURE;
};

int keygen_main(int argc, char** argv) {
    if (argc > 5) {
        return usage(argv[0]);
    }

    if (argc >= 4) {
        const mpz_class p{argv[2]};
        const mpz_class q{argv[3]};
        if (argc == 5) {
            const auto [e, d, n] = keygen(p, q, argv[4]);
            write_key(pubkey, e, n);
            write_key(prtkey, d, n);
        } else {
            const auto [e, d, n] = keygen(p, q);
            write_key(pubkey, e, n);
            write_key(prtkey, d, n);
        }
    } else if (argc == 2) {
        const auto [p, q] = generate_large_prime_numbers();
        const auto [e, d, n] = keygen(p, q);
        write_key(pubkey, e, n);
        write_key(prtkey, d, n);
    } else {
        return usage(argv[0]);
    }

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return usage(argv[0]);
    }

    const std::string mode = argv[1];
    if (mode == "keygen") {
        return keygen_main(argc, argv);
    }

    if (argc != 4 || !(mode == "e" || mode == "d")) {
        return usage(argv[0]);
    }

    const char* filename = mode == "e" ? pubkey : prtkey;
    const auto [e, n] = read_key(filename);

    auto message = read_mpz(argv[2]);
    std::cout << "message: " << message << "\n";

    auto result = crypt(message, e, n);
    std::cout << "result: " << result << "\n";
    //std::cout << "result: " << result.get_str(16) << "\n";
    int size = mpz_sizeinbase(result.get_mpz_t(), 256);
    std::cout << "size is : " << size << std::endl;
    write_mpz(argv[3], result);
}
