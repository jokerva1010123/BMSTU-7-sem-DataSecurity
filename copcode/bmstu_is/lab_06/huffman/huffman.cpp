#include "huffman.hpp"
#include "huffman_build_tree.hpp"
#include <fstream>

#define BITS_IN_BYTE 8

BitsSequence convert(const BytesSequence& bytes, uint16_t n_padding_bits) {
    if (bytes.empty()) {
        return {};
    }

    const auto n_bytes = bytes.size();
    const auto n_bits = n_bytes * BITS_IN_BYTE - n_padding_bits;

    BitsSequence bits;
    bits.reserve(n_bits);

    for (BitsSequence::size_type i = 0; i < n_bits; ++i) {
        bits.push_back(bytes[i / BITS_IN_BYTE] & (1u << (BITS_IN_BYTE - 1 - i % BITS_IN_BYTE)));
    }

    return bits;
}

BytesSequence convert(BitsSequence &bits_seq)
{
    BytesSequence bytes_seq;
    if (bits_seq.empty()){
        return {};
    }

    auto n_bits = bits_seq.size();
    auto n_bytes = (n_bits - 1) / BITS_IN_BYTE + 1;

    bytes_seq.reserve(n_bytes);

    for (BytesSequence::size_type i = 0; i < n_bytes; ++i)
    {
        BytesSequence::value_type byte{};
        for (BytesSequence::size_type j = 0; j < BITS_IN_BYTE; ++j){
            byte = (byte << 1u) | (i * BITS_IN_BYTE + j < n_bits ? bits_seq[i * BITS_IN_BYTE + j] : 0u);
        }
        bytes_seq.push_back(byte);
    }

    return bytes_seq;
}

BitsSequence compress(BytesSequence &bytes_seq, HuffmanMap &look_up_tbl){
    BitsSequence compressed_seq, symb_code;

    for (uint8_t character: bytes_seq)
    {
        symb_code = look_up_tbl.at(character);
        compressed_seq.insert(compressed_seq.end(), symb_code.begin(), symb_code.end());
    }

    return compressed_seq;
}

std::tuple<HuffmanFrequencyTable, int, BytesSequence> __huffman_compress_file(BytesSequence &bytes_buf)
{
    HuffmanFrequencyTable freq_table;
    HuffmanNode* root_huffman_tree;
    HuffmanMap look_up_tbl;
    BitsSequence enciphered_bits_seq;
    BytesSequence compressed_bytes_seq;
    int n_padded_bits = 0;

    freq_table = build_frequency_table(bytes_buf);
    root_huffman_tree = build_huffman_tree(freq_table);
    look_up_tbl = build_lookup_table(root_huffman_tree);

    enciphered_bits_seq = compress(bytes_buf, look_up_tbl);
    compressed_bytes_seq = convert(enciphered_bits_seq);

    n_padded_bits = (BITS_IN_BYTE - enciphered_bits_seq.size() % BITS_IN_BYTE) % BITS_IN_BYTE;

    std::cout << "Исходный размер файла: " << bytes_buf.size() * sizeof(uint8_t) << " байт\n";
    std::cout << "Размер таблицы частот: " << freq_table.size() * (sizeof(uint8_t) + sizeof(frequencyType)) << " байт\n";
    std::cout << "Размер сжатых данных: " << compressed_bytes_seq.size() * sizeof(uint8_t) << " байт\n";

    return {freq_table, n_padded_bits, compressed_bytes_seq};
}

int huffman_compress_file(std::string &in_file_name, std::string &out_file_name)
{
    BytesSequence in_buf;
    std::tuple<HuffmanFrequencyTable, int, BytesSequence> huffman_compress_tuple;

    in_buf = read_binary_file(in_file_name);

    if (in_buf.empty()){
        return -1;
    }

    huffman_compress_tuple = __huffman_compress_file(in_buf);

    write_binary_file(out_file_name, huffman_compress_tuple);

    return 0;
}

uint8_t code_lookup(BitsSequence::iterator &begin,
                    BitsSequence::const_iterator end,
                    const HuffmanNode* node)
{
    if (!node->left){
        throw std::invalid_argument("Неверная структура дерева: нет корня");
    }

    //идем до листа
    while (node->left)
    {
        if (!node->right){
            throw std::invalid_argument("Неверная структура дерева: нет правого узла");
        }
        node = *begin++ ? node->right : node->left;
        if (begin == end) {
            throw std::out_of_range("");
        }
    }

    return node->data.byte;
}

BytesSequence decompress(BitsSequence &bits_buf_comp, HuffmanFrequencyTable &freq_tbl)
{
    HuffmanNode *root;
    BytesSequence decompressed;
    uint8_t symbol;

    root = build_huffman_tree(freq_tbl);
    if (!root){
        throw std::invalid_argument("Нет таблицы частот символов.");
    }

    //пробегаемся по циферкам (аналог -- бит)
    for (BitsSequence::iterator begin = bits_buf_comp.begin(); ;)
    {
        try {
            symbol = code_lookup(begin, bits_buf_comp.end()+1, root);
            decompressed.emplace_back(symbol);
        }
        catch (const std::out_of_range& error) {
            break;
        }
    }

    return decompressed;
}

int huffman_decompress_file(std::string &in_file_name, std::string &out_file_name)
{
    std::tuple<HuffmanFrequencyTable, int, BytesSequence> huffman_compress_tuple;
    BytesSequence bytes_seq, decompressed_bytes_seq;
    BitsSequence encipher_bits;
    HuffmanFrequencyTable freq_tbl;

    huffman_compress_tuple = read_compressed_file(in_file_name);
    freq_tbl = std::get<0>(huffman_compress_tuple);
    bytes_seq = std::get<2>(huffman_compress_tuple);

    encipher_bits = convert(bytes_seq, std::get<1>(huffman_compress_tuple));
    decompressed_bytes_seq = decompress(encipher_bits, freq_tbl);

    write_decompressed_file(out_file_name, decompressed_bytes_seq);

    return 0;
}
