#include <iostream>
#include <fstream>
#include "huffman.h"

#include "utils.h"

using namespace std;

std::string readAllFromFile(const std::string& filename, bool& is_ok)
{
	std::string bytea;
	std::string buffer;
	std::fstream stream(filename);
	if (stream.is_open() == false)
	{
		is_ok = false;
		return string();
	}
	is_ok = true;
	int ch;
	while ( (ch = stream.get()) != EOF )
	{
		bytea += ch;
	}
	return bytea;
}

void compressAction()
{
	string filename;
	cout << "Input filename to compress > ";
	cin >> filename;
	bool is_ok;
	string initial = readAllFromFile(filename, is_ok);
	if (is_ok == false)
	{
		cout << "File does not exist" << endl;
		return;
	}
	Huffman huffman;
	string compress_filename = filename + ".compress";
	string freq_filename = filename + ".freq";
	bool success = huffman.compress(initial, compress_filename, freq_filename);
	if (success == false)
	{
		cout << "Error occured" << endl;
		return;
	}
	cout << "Compressed file was written in " << compress_filename << endl
		 << "Frequency table was written in " << freq_filename << endl;
}

void decompressAction()
{
	string filename;
	cout << "Input filename to decompress > ";
	cin >> filename;
	string decompress_filename = filename + ".compress";
	string freq_filename = filename + ".freq";
	bool is_ok;
	string compressed = readAllFromFile(decompress_filename, is_ok);
	if (is_ok == false)
	{
		cout << "File " << decompress_filename << " does not exist" << endl;
		return;
	}
	string freq = readAllFromFile(freq_filename, is_ok);
	if (is_ok == false)
	{
		cout << "File " << freq_filename << " does not exist" << endl;
		return;
	}
	Huffman huffman;
	string res_filename = huffman.decompress(compressed, freq, filename);
	if (res_filename.empty() == true)
	{
		cout << "Error occured" << endl;
		return;
	}
	cout << "Decompressed file was written in " << res_filename << endl;
}

int main()
{
	bool done = false;
	while (done == false)
	{
		cout << "Chose action (c -- compress, d -- decompress, e -- exit):" << endl;
		cout << "> ";
		string action;
		cin >> action;

		if (action == "c")
		{
			compressAction();
		}
		else if (action == "d")
		{
			decompressAction();
		}
		else if (action == "e")
		{
			done = true;
		}
		else
		{
			cout << "Unknown action" << endl;
		}
	}
	return 0;
}
