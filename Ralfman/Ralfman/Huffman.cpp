//Huffman.cpp
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "List.h"
#include "Tree.h"
#include "myException.h"

typedef unsigned char byte;

//Turning bits.
void turnByte(bool bits[8])
{
	for (int i = 0; i < 4; i++)
	{
		bool temp = bits[i];
		bits[i] = bits[7 - i];
		bits[7 - i] = temp;
	}
}

//Packing bits into bytes.
byte pack_byte(bool bits[8])
{
	byte result(0);
	for (unsigned i(8); i--;)
	{
		result <<= 1;
		result |= byte(bits[i]);
	}
	return result;
}

//Unpacking bytes into bits.
void unpack_byte(byte b, bool bits[8]) 
{
	for (unsigned i(0); i < 8; i++)
	{
		bits[i] = (b & 0x01) != 0;
		b >>= 1;
	}
}

//Byte recording.
void writeBit(unsigned char& symbol, ofstream& EncodeFile, char& position, bool* mes)
{
	mes[position] = (symbol == 0) ? 0 : 1;
	if (position == 7)
	{
		turnByte(mes);
		EncodeFile.put(pack_byte(mes));
		position = 0;
	}
	else position++;
}

//Recording last byte.
void writeLastByte(ofstream& EncodeFile, char& position, bool* mes)
{
	for (int i = position + 1; i < 8; i++)
	{
		mes[i] = 0;
	}
	turnByte(mes);
	EncodeFile.put(pack_byte(mes));
}

//Construction the Huffman tree.
void HuffmanTreeBuild(Tree* & HuffmanTree, List<Tree> & tempList)
{
	Tree* subTree1;
	Tree* subTree2;
	do
	{
		HuffmanTree = tempList.deleteHead();
		if (!tempList.isEmpty())
		{
			subTree1 = tempList.deleteHead();
			subTree2 = new Tree;
			subTree2->root_->frequency_ = subTree1->root_->frequency_ + HuffmanTree->root_->frequency_;
			subTree2->root_->left_ = HuffmanTree->root_;
			subTree2->root_->right_ = subTree1->root_;
			tempList.insert(*subTree2);
		}
	} while (!tempList.isEmpty());
}

//Construction code of symbols by Huffman tree.
void Tree::buildCode(Node* root, unsigned char symbol, string temp, string & code) const
{
	if (root)
	{
		if (!root->left_ && !root->right_ && root->symbol_ == symbol)
		{
			code = temp;
		}
		else
		{
			buildCode(root->left_, symbol, temp + "0", code);
			buildCode(root->right_, symbol, temp + "1", code);
		}
	}
}

void encode(string ifile, string ofile)
{
	int frequency[256] = { 0 };

	ifstream TextFile;
	TextFile.open(ifile);
	if (!TextFile) throw DataError("File not opened!", ifile, 0);



	//-------------Calculation of frequency-------------------
	char currentSymbol;
	while (TextFile.get(currentSymbol))
	{
		frequency[static_cast<unsigned char>(currentSymbol)]++;
	}



	//-------------Creating list of all used symbols------------------
	List<Tree> tempList;
	Tree* HuffTree;
	ofstream TableFile;
	TableFile.open("Table.txt", ios::binary);
	if (!TableFile) throw ("File not opened!", "Table.txt", 0);

	for (int i = 0; i < 256; ++i) 
	{
		if (frequency[i] != 0)
		{
			HuffTree = new Tree;
			HuffTree->root_->frequency_ = frequency[i];
			HuffTree->root_->symbol_	= static_cast<char>(i);
			tempList.insert(*HuffTree);
			TableFile << i << " " << frequency[i] << endl;
		}
	}



	//--------------Construction Huffman tree by list----------------------
	HuffmanTreeBuild(HuffTree, tempList);



	//--------------Construction the code table----------------------------
	TextFile.clear();
	TextFile.seekg(0);

	ofstream EncodeFile;
	EncodeFile.open(ofile, ios::binary);
	if (!EncodeFile) throw DataError("File not opened!", ofile, 0);

	string table[256] = {""};
	unsigned char symbol;
	for (int number = 0; number < 256; ++number) {
		symbol = static_cast<unsigned char>(number);
		HuffTree->buildCode(HuffTree->root_, symbol, "", table[number]);
	}



	//---------------Writing to the file "Encoded.txt"----------------------
	unsigned char currentBit;
	char position = 0;
	bool mes[8] = { 0 };

	while (TextFile.get(currentSymbol))
	{
		for (unsigned int i = 0; i < table[static_cast<unsigned char>(currentSymbol)].size(); ++i)
		{
			if (table[static_cast<unsigned char>(currentSymbol)].at(i) == '0')
				currentBit = 0;
			if (table[static_cast<unsigned char>(currentSymbol)].at(i) == '1')
				currentBit = 1;
			writeBit(currentBit, EncodeFile, position, mes);
		}
	}

	writeLastByte(EncodeFile, position, mes);

	TableFile << 0 << " ";
	TableFile << static_cast<int>(position);		//number of real bits in the last byte

	TableFile.close();
	TextFile.close();
	EncodeFile.close();
}



void decode(string ifile, string ofile)
{
	ifstream TextFile;
	TextFile.open(ifile, ios::binary);
	if (!TextFile) throw DataError("File not opened!", ifile, 0);

	ifstream TableFile;
	TableFile.open("Table.txt");
	if (!TableFile) throw DataError("File not opened!", "Table.txt", 0);

	//------------------Creating an array of symbols, with their associated frequency---------------------
	int frequency[256] = { 0 };
	int tempFrequency = 0;

	int i = 0;
	TableFile >> i;
	while (i != 0) 
	{
		TableFile >> tempFrequency;
		frequency[i] = tempFrequency;
		TableFile >> i;
	}



	//-----------------Huffman tree restoring an array of symbols with their frequency-------------------
	Tree* huffTree;
	List<Tree> tempList;
	for (i = 0; i < 256; i++)
	{
		if (frequency[i] != 0) 
		{
			huffTree = new Tree;
			huffTree->root_->frequency_ = frequency[i];
			huffTree->root_->symbol_ = static_cast<char>(i);
			tempList.insert(*huffTree);
		}
	}



	//--------------Construction Huffman tree by list----------------------
	HuffmanTreeBuild(huffTree, tempList);



	//----------------Reading the encoded file, decode it, and writeBit to the end file---------------------
	ofstream DecodeFile;
	DecodeFile.open(ofile, ios::binary);

	char currentSymbol;
	bool mes[8] = { 0 };
	Tree::Node* root = huffTree->root_;

	while (TextFile.get(currentSymbol))
	{
		if (TextFile.peek() == EOF) break;		//if this is the last byte
		unpack_byte(static_cast<unsigned char>(currentSymbol), mes);
		turnByte(mes);
		for (i = 0; i < 8; i++) 
		{
			if (!mes[i])
				root = root->left_;
			else
				root = root->right_;
			if (root->symbol_ != NULL)
			{
				DecodeFile << root->symbol_;
				root = huffTree->root_;
			}
		}
	}



	//Writing the last byte.
	int bits = 0;
	TableFile >> bits;
	unpack_byte(static_cast<unsigned char>(currentSymbol), mes);
	turnByte(mes);
	for (int i = 0; i < bits; i++) 
	{
		if (!mes[i])
			root = root->left_;
		else
			root = root->right_;
		if (root->symbol_ != NULL) 
		{
			DecodeFile << root->symbol_;
			root = huffTree->root_;
		}
	}

	TableFile.close();
	TextFile.close();
	DecodeFile.close();
}

