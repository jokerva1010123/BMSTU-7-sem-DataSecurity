#include <fstream>
#include "io.hpp"

BytesSequence read_binary_file(std::string &in_file_name)
{
    std::ifstream input_file(in_file_name, std::ios::binary);
    if (!input_file){
        return {};
    }

    BytesSequence in_buf(
       (std::istreambuf_iterator<char>(input_file)),
       (std::istreambuf_iterator<char>())
    );

    input_file.close();

    return in_buf;
}

std::tuple<HuffmanFrequencyTable, int, BytesSequence>
    read_compressed_file(std::string &in_file_name)
{
    uint16_t frequency_table_size{};
    uint8_t n_padded_bits{};
    HuffmanFrequencyTable frequency_table;
    std::ifstream in_file(in_file_name, std::ios::binary);
    HuffmanFrequencyTable::key_type character{};
    HuffmanFrequencyTable::mapped_type frequency{};
    BytesSequence encoded_bytes;

    if (!in_file) {
        std::cerr << "Не удалось открыть исходный файл.\n";
    }
    else{
        in_file.read(reinterpret_cast<char*>(&frequency_table_size), sizeof frequency_table_size);
        in_file.read(reinterpret_cast<char*>(&n_padded_bits), sizeof n_padded_bits);

        for (decltype(frequency_table_size) i = 0; i < frequency_table_size; ++i)
        {
            in_file.read(reinterpret_cast<char*>(&character), sizeof character);
            in_file.read(reinterpret_cast<char*>(&frequency), sizeof frequency);
            frequency_table[character] = frequency;
        }

        encoded_bytes = BytesSequence(
                (std::istreambuf_iterator<char>(in_file)),
                (std::istreambuf_iterator<char>()));
        in_file.close();
    }

    return {frequency_table, n_padded_bits, encoded_bytes};
}

void write_binary_file(std::string &out_file_name,
                       std::tuple<HuffmanFrequencyTable, int, BytesSequence> &out_buf)
{
    std::ofstream out_file(out_file_name, std::ios::binary);
    if (out_file.fail()){
        std::cerr << "Не удалось записать в файл.\n";
    }
    else{
        auto frequency_table = std::get<0>(out_buf);
        const auto frequency_table_size = static_cast<uint16_t>(frequency_table.size());
        out_file.write(reinterpret_cast<const char*>(&frequency_table_size), sizeof frequency_table_size);

        auto n_padded_bits = static_cast<uint8_t>(std::get<1>(out_buf));
        out_file.write(reinterpret_cast<const char*>(&n_padded_bits), sizeof n_padded_bits);

        for (auto [character, frequency]: frequency_table){
            out_file.write(reinterpret_cast<const char*>(&character), sizeof character);
            out_file.write(reinterpret_cast<const char*>(&frequency), sizeof frequency);
        }

        auto compressed_bytes = std::get<2>(out_buf);
        out_file.write(reinterpret_cast<const char *>(&compressed_bytes[0]), compressed_bytes.size());

        out_file.close();
    }
}

void write_decompressed_file(std::string &out_file_name, BytesSequence &byte_seq)
{
    std::ofstream output_file(out_file_name, std::ios::binary);
    if (!output_file) {
        throw std::invalid_argument("Невозможно открыть сжатый файл.");
    }

    output_file.write(reinterpret_cast<const char*>(&byte_seq[0]), byte_seq.size());
    output_file.close();
}
