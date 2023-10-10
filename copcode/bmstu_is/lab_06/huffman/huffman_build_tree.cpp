#include "huffman_build_tree.hpp"

HuffmanFrequencyTable build_frequency_table(const BytesSequence &bytes_seq)
{
    HuffmanFrequencyTable freq_table;
    for (auto byte: bytes_seq){
        ++freq_table[byte];
    }

    /*for (auto &[byte, index]: freq_table){
        std::cout << "byte: " << (int)byte << " freq[byte]: " << index << "\n";
    }*/

    return freq_table;
}

HuffmanNode* build_huffman_tree(HuffmanFrequencyTable &freq_table)
{
    HuffmanPriorityQueue prior_queue;

    for (auto [character, frequency]: freq_table){
        auto node = new HuffmanNode{{character, frequency}};
        prior_queue.push(node);
    }
    // хвост -- 7 4 3 2 1 1 -- голова   =>   7 4 3 2 2  => 7 4 4 3 => 7 7 4 => 11 7 => 18
    //            2             4                        18
    //         /    \         /   \                    /   \
    //        1      1       2     2                  7    11
    //                           /   \               / \   / \
    //                          1     1             3  4  4  7
    while (prior_queue.size() > 1){
        auto left = prior_queue.top();
        prior_queue.pop();

        auto right = prior_queue.top();
        prior_queue.pop();

        auto node = new HuffmanNode{{0, left->data.frequency + right->data.frequency}, left, right};
        prior_queue.push(node);
    }

    return prior_queue.empty() ? nullptr : prior_queue.top();
}

HuffmanMap build_lookup_table(HuffmanNode *root)
{
    HuffmanMap look_up;
    std::deque<std::pair<HuffmanNode*, BitsSequence>> deque{{root, {}}};
    BitsSequence code_left, code_right;

    //root -- 18;
    //deque: 18 => 7  11  =>  11    3    4    =>   4    7     3    4
    //код    {}   {0} {1}     {1} {00} {01}      {10} {11}  {00} {01}
    while (!deque.empty())
    {
        auto [node, code] = deque.front();
        deque.pop_front();
        //если узел не является листом
        if (node->left){
            code_left = code;
            code_right = code;
            code_left.push_back(0);
            code_right.push_back(1);
            deque.emplace_back(node->left, code_left);
            deque.emplace_back(node->right, code_right);
        }
        else{
            look_up.emplace(node->data.byte, code);
        }
    }

    /*std::cout << "LOOK_UP_TABLE\n";
    for(auto n : look_up){
        std::cout << (int)n.first << ' ';
        for (int i = 0; i < n.second.size(); ++i){
            std::cout << n.second[i];
        }
        std::cout << "\n";
    }*/
    return look_up;
}

