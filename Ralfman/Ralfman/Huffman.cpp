#include <fstream>
#include <iostream>
using namespace std;
#include <string>
#include <fcntl.h>
#include "List.h"
#include "Tree.h"
#include "NotFoundFile.h"

typedef unsigned char byte;

//Turning bits.
void turn(bool bits[8])
{
	for (int i = 0; i < 4; i++) {
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
void unpack_byte(byte b, bool bits[8]) {
	for (unsigned i(0); i < 8; i++) {
		bits[i] = (b & 0x01) != 0;
		b >>= 1;
	}
}

//Byte recording.
void write(unsigned char & symbol, ofstream & E, char & position, bool* mes)
{
	mes[position] = (symbol == 0) ? 0 : 1;
	if (position == 7)
	{
		turn(mes);
		E.put(pack_byte(mes));
		position = 0;
	}
	else position++;
}

//Recording last byte.
void writelast(ofstream & E, char & position, bool* mes)
{
	for (int i = position + 1; i < 8; i++) {
		mes[i] = 0;
	}
	turn(mes);
	E.put(pack_byte(mes));
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
void Tree::build(Node* root, unsigned char symbol, string temp, string & code) const
{
	if (root)
	{
		if (!root->left_ && !root->right_ && root->symbol_ == symbol)
		{
			code = temp;
		}
		else
		{
			build(root->left_, symbol, temp + "0", code);
			build(root->right_, symbol, temp + "1", code);
		}
	}
}

//---------------------------------------------ENCODING-------------------------------------------------------
void encode(string ifile, string ofile)
{
	int kolSymbols = 0;
	int arrSymbols[256] = { 0 };
	ifstream F;
	F.open(ifile);
	if (F)
	{
		//------------------------------Calculation of frequency-----------------------------------------------
		char currentSymbol;
		while (F.get(currentSymbol))
		{
			arrSymbols[unsigned char(currentSymbol)]++;
			kolSymbols++;
		}
		cout << kolSymbols << endl;
		//----------------------------Creating list of all used symbols-----------------------------------------
		List<Tree> tempList;
		Tree* HuffTree;
		ofstream T;
		T.open("Table.txt", ios::binary);

		for (int i = 0; i < 256; ++i)
		{
			if (arrSymbols[i] != 0)
			{
				HuffTree = new Tree;
				HuffTree->root_->frequency_ = arrSymbols[i];
				HuffTree->root_->symbol_ = char(i);
				tempList.insert(*HuffTree);
				T << i << " " << arrSymbols[i] << endl;
			}
		}

		//----------------------------Construction Huffman tree by list-----------------------------------------
		HuffmanTreeBuild(HuffTree, tempList);

		//-------------------------------Construction the code table---------------------------------------------
		F.clear();
		F.seekg(0);
		ofstream E;
		E.open(ofile, ios::binary);

		string table[256];
		unsigned char symbol;
		for (int number = 0; number < 256; ++number) {
			table[number] = "";
			symbol = unsigned char(number);
			HuffTree->build(HuffTree->root_, symbol, "", table[number]);
		}

		//------------------------------Writing to the file "Encoded.txt"----------------------------------------
		unsigned char ch2;
		char position = 0;
		bool mes[8] = { 0 };
		int numBits = 0;
		int numBytes = 0;
		int numKBytes = 0;
		int numMBytes = 0;

		while (F.get(currentSymbol))
		{
			for (unsigned int i = 0; i < table[unsigned char(currentSymbol)].size(); ++i)
			{
				if (table[unsigned char(currentSymbol)].at(i) == '0')
					ch2 = 0;
				if (table[unsigned char(currentSymbol)].at(i) == '1')
					ch2 = 1;
				write(ch2, E, position, mes);
				numBits++;
				if ((numBits % 8 == 0)&&(numBits != 0)) {
					numBits = 0;
					numBytes++;
				}
				if ((numBytes % 1024 == 0) && (numBytes != 0)) {
					numBytes = 0;
					numKBytes++;
				}
				if ((numKBytes % 1024 == 0) && (numKBytes != 0)) {
					numKBytes = 0;
					numMBytes++;
				}
			}
		}

		writelast(E, position, mes);

		T << 0 << " ";
		T << numBits   << " " 
		  << numBytes  << " " 
		  << numKBytes << " "
		  << numMBytes;

		T.close();
		F.close();
		E.close();
	}

	else throw NotFoundFile();
}

//---------------------------------------------DECODING-------------------------------------------------------
void decode(string ifile, string ofile) {
	ifstream F;
	F.open(ifile, ios::binary);
	if (F)
	{
		//------------------Creating an array of symbols, with their associated frequency---------------------
		ifstream T;
		T.open("Table.txt");
		int arrSymbols[256] = { 0 };
		int i = 0;
		int tempFrequency = 0;

		int numBits = 0;
		int numBytes = 0;
		int numKBytes = 0;
		int numMBytes = 0;

		T >> i;
		while (i != 0) {
			T >> tempFrequency;
			arrSymbols[i] = tempFrequency;
			T >> i;
		}

		T >> numBits 
		  >> numBytes
		  >> numKBytes
		  >> numMBytes;
		cout << "Bits: " << numBits << endl;
		cout << "Bytes: " << numBytes << endl;
		cout << "Kilobytes: " << numKBytes << endl;
		cout << "MegaBytes: " << numMBytes << endl;

		T.close();

		//-----------------Huffman tree restoring an array of symbols with their frequency-------------------
		Tree* huffTree;
		List<Tree> tempList;
		for (i = 0; i < 256; i++) {
			if (arrSymbols[i] != 0) {
				huffTree = new Tree;
				huffTree->root_->frequency_ = arrSymbols[i];
				huffTree->root_->symbol_ = char(i);
				tempList.insert(*huffTree);
			}
		}

		HuffmanTreeBuild(huffTree, tempList);

		//----------------Reading the encoded file, decode it, and write to the end file---------------------
		ofstream D;
		D.open(ofile, ios::binary);

		char currentSymbol;
		bool mes[8] = { 0 };
		i = 0;
		Tree::Node* root = huffTree->root_;

		long long fullBytes = numBytes + numKBytes * 1024 + numMBytes * 1024 * 1024;
		
		//Writing all bytes without final.
		while (fullBytes != 0) {
			F.get(currentSymbol);
			unpack_byte(unsigned char(currentSymbol), mes);
			turn(mes);
			for (i = 0; i < 8; i++) {
				if (!mes[i])
					root = root->left_;
				else
					root = root->right_;
				if (root->symbol_ != NULL) {
					D << root->symbol_;
					root = huffTree->root_;
				}
			}
			fullBytes--;
		}

		//Writing the last byte.
		F.get(currentSymbol);
		unpack_byte(unsigned char(currentSymbol), mes);
		turn(mes);
		for (int i = 0; i < numBits; i++) {
			if (!mes[i])
				root = root->left_;
			else
				root = root->right_;
			if (root->symbol_ != NULL) {
				D << root->symbol_;
				root = huffTree->root_;
			}
		}
		
		F.close();
		D.close();
	}
	else throw NotFoundFile();
}

