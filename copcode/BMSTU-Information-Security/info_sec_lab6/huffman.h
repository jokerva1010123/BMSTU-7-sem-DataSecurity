#pragma once

#include <string>
#include <array>
#include <inttypes.h>
#include <algorithm>

#include "binarytree.h"
#include "frequency_table.h"

using namespace std;

using CodePair = pair<uint8_t, string>;
using CodesTable = vector<CodePair>;

class Huffman
{
public:

	bool compress(string initial, const string compressed_filename, const string freq_filename);
	string decompress(string compressed, string freq, const string filename);
private:
	string __decompress(string bin_compressed, const BinaryTree& tree);

	pair<FreqTable, uint8_t> parseString(string str);
	FreqPair getFreqFromString(string str);
	void pack(const string bitstr, ofstream& fout);
	string findCode(const CodesTable& codes_table, uint8_t symb);
	int comressToFile(const CodesTable& codes_table, const string compressed_filename, string initial);
	bool freqToFile(const FreqTable& table, const string freq_filename, uint8_t add_bits);
	FreqTable createFreqTable(string str);
	void sortFreqTable(FreqTable& table);
	std::unique_ptr<BinaryTree> buildTree(const FreqTable& freq_table);
	std::shared_ptr<BinaryTreeNode> mergeNodes(std::shared_ptr<BinaryTreeNode> left_node, std::shared_ptr<BinaryTreeNode> right_node);
	CodesTable buildCodesTable(const BinaryTree &head);
	void treeSearch(BinaryTreeNode* node);
	vector<std::shared_ptr<BinaryTreeNode> > copy(u_int64_t from, uint64_t to, vector<std::shared_ptr<BinaryTreeNode> > source);
	vector<std::shared_ptr<BinaryTreeNode>> sortBuffer(vector<std::shared_ptr<BinaryTreeNode> > buffer);
};

