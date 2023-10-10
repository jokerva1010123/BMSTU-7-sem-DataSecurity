#include "huffman.h"
#include <fstream>
#include <assert.h>
#include <math.h>

constexpr char LowByte = '0';
constexpr char HightByte = '1';

#include "utils.h"

/* \n 100
 * H 101
 * e 110
 * o 111
 * l 0
 */

/*		{}
 *     /  \
 *     0  1
 *	left  right
 */

uint8_t _binStrToUlong(const string& str);

bool Huffman::compress(string initial, const string compressed_filename, const string freq_filename)
{
	BinaryTree* raw_tree;
	{
		FreqTable freq_table = createFreqTable(initial);
		std::unique_ptr<BinaryTree> tree = buildTree(freq_table);
		raw_tree = tree.get();
		if (tree == nullptr)
		{
			return false;
		}
		CodesTable codes_table = buildCodesTable(*tree);

//		for (uint64_t i = 0, n = codes_table.size(); i < n; ++i)
//		{
//			cout << codes_table[i].first << " " << codes_table[i].second << endl;
//		}

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
	}

	return true;
}

string Huffman::decompress(string compressed, string freq, const string filename)
{
	pair<FreqTable, uint8_t> freq_pair = parseString(freq);
	//cout << compressed << endl;
	string binary_compressed = strToBin(compressed);
	binary_compressed = binary_compressed.substr(0, binary_compressed.size() - freq_pair.second);
	auto tree = buildTree(freq_pair.first);
//	CodesTable codes_table = buildCodesTable(*tree);

//	for (uint64_t i = 0, n = codes_table.size(); i < n; ++i)
//	{
//		cout << codes_table[i].first << " " << codes_table[i].second << endl;
//	}

	string decompressed_string = __decompress(binary_compressed, *tree);
	//cout << decompressed_string << endl;

	string decompressed_filename = filename + ".decompressed";

	int is_ok = strToFile(decompressed_filename, decompressed_string);
	if (is_ok == -1)
	{
		cout << "Cannot write to file" << endl;
		return string();
	}
	return decompressed_filename;
}

string Huffman::__decompress(string bin_compressed, const BinaryTree &tree)
{
	string res;
	std::shared_ptr<BinaryTreeNode> cur_node = tree.mHead;
//	for (auto riter = bin_compressed.rbegin(); riter != bin_compressed.rend(); ++riter)
	for (auto iter = bin_compressed.begin(); iter != bin_compressed.end(); )
	{
		auto bin_val = *iter;
		if (bin_val == LowByte)
		{
			if (cur_node->mLeft != nullptr)
			{
				cur_node = cur_node->mLeft;
				iter++;
			}
			else
			{
				res += cur_node->mValue.first;
				cur_node = tree.mHead;
			}
		}
		else
		{
			if (cur_node->mRight != nullptr)
			{
				cur_node = cur_node->mRight;
				iter++;
			}
			else
			{
				res += cur_node->mValue.first;
				cur_node = tree.mHead;
			}
		}
	}
	return res;
}

pair<FreqTable, uint8_t> Huffman::parseString(string str) // BAD
{
	FreqTable freq_table;
	uint8_t add_bits;
	vector<string> split_freq = split(str, '\n');
	for (uint64_t i = 0, n = split_freq.size(); i < n - 1; ++i)
	{
		string cur_str = split_freq.at(i);
		vector<string> cur_split = split(cur_str, '}');
		string str_symb = cur_split[0].substr(1, cur_split[0].size() - 1);
		string str_freq = cur_split[1].substr(1, cur_split[1].size() - 1);
		FreqPair pair;
		pair.first = stoi(str_symb); //(uint8_t) str_symb[0];
		pair.second = stoull(str_freq);
		//cout << pair.first << " " << pair.second << endl;
		freq_table[i] = pair;
	}
	string str_add = split_freq[split_freq.size() - 1].substr(1, split_freq[split_freq.size() - 1].size() - 2);
	add_bits = stoull(str_add);
	pair<FreqTable, uint8_t> res = pair<FreqTable, uint8_t>(freq_table, add_bits);
	return res;
}

FreqPair Huffman::getFreqFromString(string str)
{
	assert(str.size() >= 3);
	FreqPair pair;
	pair.first = str[1];
	string freq_str = str.substr(4, str.size() - 4);
	uint64_t freq = stoll(freq_str);
	pair.second = freq;
	return pair;
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
		string code = findCode(codes_table, symb);
//		std::reverse(code.begin(), code.end());
		full_code += code;
	}

	uint16_t additional_bits = 8 * (full_code.size() / 8 + 1) - full_code.size();
	cout << "Add " << additional_bits << " bit(s)" << endl;
	//fout << "(" << additional_bits << ")";
	for (uint16_t i = 0; i < additional_bits; ++i)
	{
		full_code.push_back(LowByte);
	}

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
			fout << "{" <<  uint16_t(i.first[0]) << "} " << i.second << "\n";
			//cout << "{" <<  uint16_t(i.first[0]) << "} " << i.second << "\n";
		}
	}
	fout << "(" << uint16_t(add_bits) << ")";
	//cout << "(" << uint16_t(add_bits) << ")";
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

std::unique_ptr<BinaryTree> Huffman::buildTree(const FreqTable &freq_table)
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
		auto tree = std::make_unique<BinaryTree>();
		auto node = std::make_shared<BinaryTreeNode>();
		node->mValue = *first_nonull;
		tree->mHead = node;
		tree->mLists.push_back(node);
		return tree;
	}
	else
	{
		--first_nonull;
		auto tree = std::make_unique<BinaryTree>();

		while (first_nonull >= freq_table.begin())
		{
			auto node = std::make_shared<BinaryTreeNode>();
			node->mValue = FreqPair(first_nonull->first, first_nonull->second);
			tree->mLists.push_back(node);
			first_nonull--;
		}
		vector<std::shared_ptr<BinaryTreeNode>> buffer;
		int64_t lists_count = tree->mLists.size();
		for (int64_t i = 0; i < lists_count; ++i)
		{
			buffer.push_back(tree->mLists.at(i));
		}
		vector<std::shared_ptr<BinaryTreeNode>> cur_buff = buffer;
		while (buffer.size() > 1)
		{
			cur_buff = copy(2, buffer.size() - 1, buffer);
			auto merged_node = mergeNodes(buffer.at(0), buffer.at(1));
			cur_buff.push_back(merged_node);
			cur_buff = sortBuffer(cur_buff);
			buffer = cur_buff;
		}
		tree->mHead = buffer.at(0);
		return tree;
	}
}

std::shared_ptr<BinaryTreeNode> Huffman::mergeNodes(std::shared_ptr<BinaryTreeNode> left_node, std::shared_ptr<BinaryTreeNode> right_node)
{
	auto merge_node = std::make_shared<BinaryTreeNode>();
	merge_node->mLeft = left_node;
	merge_node->mRight = right_node;
	left_node->mParent = merge_node;
	right_node->mParent = merge_node;
	string merge_string = string(left_node->mValue.first) + string(right_node->mValue.first);
	uint64_t merge_freq = left_node->mValue.second + right_node->mValue.second;
	merge_node->mValue = FreqPair(merge_string, merge_freq);
	return merge_node;
}

CodesTable Huffman::buildCodesTable(const BinaryTree& head)
{
	CodesTable table;
	for (auto& list : head.mLists) // Листики дерева
	{
		string code;
		auto prev_node = list;
		auto cur_node = list->mParent;
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

vector<std::shared_ptr<BinaryTreeNode>>
Huffman::copy(u_int64_t from, uint64_t to, vector<std::shared_ptr<BinaryTreeNode>> source)
{
	vector<std::shared_ptr<BinaryTreeNode>> copy;
	for (uint64_t i = from; i <= to; ++i)
	{
		copy.push_back(source.at(i));
	}
	return copy;
}

vector<std::shared_ptr<BinaryTreeNode> > Huffman::sortBuffer(vector<std::shared_ptr<BinaryTreeNode>> buffer)
{
	sort(buffer.begin(), buffer.end(), [](auto a, auto b){
		return a->mValue.second < b->mValue.second;
	});
	vector<vector<std::shared_ptr<BinaryTreeNode>>::iterator> bounds;

	auto prev = buffer.begin();
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
		sort(bounds.at(i), bounds.at(i + 1) + 1, [](auto a, auto b){
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
		uint32_t factor = (str[j] == HightByte) ? 1 : 0;
		res += factor * pow(2, i);
	}
	return res;
}
