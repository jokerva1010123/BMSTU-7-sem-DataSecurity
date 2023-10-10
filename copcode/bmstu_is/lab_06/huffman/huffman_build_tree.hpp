#ifndef _HUFFMAN_BUILD_TREE_HPP_
#define _HUFFMAN_BUILD_TREE_HPP_

#include <queue>

#include "huffman_defines.hpp"

struct HuffmanNodeData
{
    uint8_t byte;
    frequencyType frequency;
};

template <typename T>
struct BinaryTreeNode
{
    T data = T{};
    BinaryTreeNode *left = nullptr;
    BinaryTreeNode *right = nullptr;
};
using HuffmanNode = BinaryTreeNode<HuffmanNodeData>;


struct HuffmanNodeCompare
{
    bool operator()(HuffmanNode *left, HuffmanNode *right){
        return left->data.frequency > right->data.frequency;
    }
};

using HuffmanPriorityQueue = std::priority_queue<HuffmanNode *, std::vector<HuffmanNode*>, HuffmanNodeCompare>;

HuffmanFrequencyTable build_frequency_table(const BytesSequence &bytes_seq);
HuffmanNode* build_huffman_tree(HuffmanFrequencyTable &freq_table);
HuffmanMap build_lookup_table(HuffmanNode *root);

#endif
