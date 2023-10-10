#include "huffman.h"
#include <fstream>
#include <math.h>

/*		{}
 *     /  \
 *     0  1
 *	left  right
 */

uint8_t _binStrToUlong(const string& str);

bool Huffman::compress(string initial, const string compressed_filename, const string freq_filename)
{
	FreqTable freq_table = createFreqTable(initial);
	BinaryTree* tree = buildTree(freq_table);
	if (tree == nullptr)
	{
		return false;
	}
	CodesTable codes_table = buildCodesTable(tree);

	int success = comressToFile(codes_table, compressed_filename, initial);
	if (success == -1)
	{
		cout << "Unable to write in compressed file" << endl;
		return false;
	}

	bool is_ok = freqToFile(freq_table, freq_filename, success);
	if (is_ok == false)
	{
		cout << "Unable to open freq file" << endl;
		return false;
	}

	return true;
}

void Huffman::pack(const string bitstr, ofstream &fout)
{
	uint64_t count = bitstr.size() / 8;

	for (uint64_t i = 0; i < count; ++i)
	{
		string cur_byte = bitstr.substr(i * 8, 8);
		uint8_t c = _binStrToUlong(cur_byte);
		fout.write((char*)&c, sizeof (c));
	}
}

string Huffman::findCode(const CodesTable &codes_table, uint8_t symb)
{
	auto iter = find_if(codes_table.begin(), codes_table.end(), [&](CodePair code_pair){
		return code_pair.first == symb;
	});
	if (iter == codes_table.end())
	{
		return string();
	}
	return (*iter).second;
}

int Huffman::comressToFile(const CodesTable &codes_table, const string compressed_filename, string initial)
{
	ofstream fout(compressed_filename, ios_base::trunc | ios_base::binary);
	if (fout.is_open() == false)
	{
		return -1;
	}
	string full_code;
	for (uint8_t symb : initial)
	{
		full_code += findCode(codes_table, symb);
	}

	cout << full_code << endl;
	uint16_t additional_bits = 8 * (full_code.size() / 8 + 1) - full_code.size();
	cout << "Add " << additional_bits << " bit(s)" << endl;
	//fout << "(" << additional_bits << ")";
	for (uint16_t i = 0; i < additional_bits; ++i)
	{
		full_code.push_back('0');
	}

	cout << full_code << endl;

	pack(full_code, fout);

	fout.close();
	return additional_bits;
}

bool Huffman::freqToFile(const FreqTable &table, const string freq_filename, uint8_t add_bits)
{
	ofstream fout(freq_filename, ios_base::trunc);
	if (fout.is_open() == false)
	{
		return false;
	}
	for (auto i : table)
	{
		if (i.second != 0)
		{
			fout << "{" << i.first << "} " << i.second << "\n";
		}
	}
	fout << "(" << add_bits << ")";
	fout.close();
	return true;
}

FreqTable Huffman::createFreqTable(string str)
{
	FreqTable freq_table; // pair <symbol, freq>

	for (uint16_t i = 0; i < ASCII_SIZE; ++i)
	{
		FreqPair p;
		p.first = i;
		p.second = 0;
		freq_table[i] = p;
	}

	for (uint8_t symb : str)
	{
		freq_table[symb].second++;
	}

	sortFreqTable(freq_table);
	return freq_table;
}

void Huffman::sortFreqTable(FreqTable &table)
{
	sort(table.begin(), table.end(), [](FreqPair a, FreqPair b) {
		return a.second > b.second;
	});
}

BinaryTree* Huffman::buildTree(const FreqTable &freq_table)
{
	auto first_nonull = find_if(freq_table.begin(), freq_table.end(), [] (FreqPair p){
		return p.second == 0;
	});
	if (first_nonull == freq_table.begin())
	{
		return nullptr;
	}
	else if (first_nonull == freq_table.begin() + 1)
	{
		first_nonull--;
		BinaryTree* tree = new BinaryTree();
		BinaryTreeNode* node = new BinaryTreeNode();
		node->mValue = *first_nonull;
		tree->mHead = node;
		tree->mLists.push_back(node);
		return tree;
	}
	else
	{
		--first_nonull;
		BinaryTree* tree = new BinaryTree();

		while (first_nonull >= freq_table.begin())
		{
			BinaryTreeNode* node = new BinaryTreeNode();
			node->mValue = FreqPair(first_nonull->first, first_nonull->second);
			tree->mLists.push_back(node);
			first_nonull--;
		}
		vector<BinaryTreeNode*> buffer;
		int64_t lists_count = tree->mLists.size();
		for (int64_t i = 0; i < lists_count; ++i)
		{
			buffer.push_back(tree->mLists.at(i));
		}
		vector<BinaryTreeNode*> cur_buff = buffer;
		while (buffer.size() > 1)
		{

			cur_buff = copy(2, buffer.size() - 1, buffer);
			BinaryTreeNode* merged_node = mergeNodes(buffer.at(0), buffer.at(1));
			cur_buff.push_back(merged_node);
			cur_buff = sortBuffer(cur_buff);
			buffer = cur_buff;
		}
		tree->mHead = buffer.at(0);
		return tree;
	}
}

BinaryTreeNode *Huffman::mergeNodes(BinaryTreeNode *left_node, BinaryTreeNode *right_node)
{
	BinaryTreeNode* merge_node = new BinaryTreeNode();
	merge_node->mLeft = left_node;
	merge_node->mRight = right_node;
	left_node->mParent = merge_node;
	right_node->mParent = merge_node;
	string merge_string = string(left_node->mValue.first) + string(right_node->mValue.first);
	uint64_t merge_freq = left_node->mValue.second + right_node->mValue.second;
	merge_node->mValue = FreqPair(merge_string, merge_freq);
	return merge_node;
}

CodesTable Huffman::buildCodesTable(BinaryTree *head)
{
	CodesTable table;
	for (BinaryTreeNode* list : head->mLists)
	{
		string code;
		BinaryTreeNode* prev_node = list;
		BinaryTreeNode* cur_node = list->mParent;
		while (cur_node != nullptr)
		{
			if (cur_node->mLeft == prev_node)
			{
				code = "0" + code;
			}
			else if (cur_node->mRight == prev_node)
			{
				code = "1" + code;
			}
			prev_node = cur_node;
			cur_node = cur_node->mParent;
		}
		CodePair pair = CodePair(list->mValue.first[0], code);
		table.push_back(pair);
	}
	return table;
}

vector<BinaryTreeNode *> Huffman::copy(u_int64_t from, uint64_t to, vector<BinaryTreeNode *> source)
{
	vector<BinaryTreeNode *> copy;
	for (uint64_t i = from; i <= to; ++i)
	{
		copy.push_back(source.at(i));
	}
	return copy;
}

vector<BinaryTreeNode *> Huffman::sortBuffer(vector<BinaryTreeNode *> buffer)
{
	sort(buffer.begin(), buffer.end(), [](BinaryTreeNode* a, BinaryTreeNode* b){
		return a->mValue.second < b->mValue.second;
	});
	vector<vector<BinaryTreeNode *>::iterator> bounds;

	vector<BinaryTreeNode *>::iterator prev = buffer.begin();
	bounds.push_back(prev);
	for (auto iter = buffer.begin(); iter < buffer.end(); iter++)
	{
		if ((*prev)->mValue.second != (*iter)->mValue.second)
		{
			bounds.push_back(iter - 1);
			bounds.push_back(iter);
			prev = iter;
		}
	}
	bounds.push_back(buffer.end() - 1);
	for (uint64_t i = 0, n = bounds.size(); i < n; i += 2)
	{
		sort(bounds.at(i), bounds.at(i + 1) + 1, [](BinaryTreeNode* a, BinaryTreeNode* b){
			return a->mValue.first.size() > b->mValue.first.size();
		});
	}
	return buffer;
}

uint8_t _binStrToUlong(const string& str)
{
	uint32_t res = 0;
	for (int16_t i = str.size() - 1, j = 0; i >= 0; --i, ++j)
	{
//		uint32_t factor = strtol(str.data() + j, nullptr, 10);
		uint32_t factor = (str[j] == '1') ? 1 : 0;
		res += factor * pow(2, i);
	}
	return res;
}
