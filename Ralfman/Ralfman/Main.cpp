//HUFFMAN CODE
//Main.cpp
#include "myException.h"
#include <string>
#include <iostream>
using namespace std;

void encode(string ifile, string ofile);

void decode(string ifile, string ofile);

int main() {
	setlocale(LC_ALL, "rus");

	try {
		string textFile = "WarAndPiece.txt";
		string encodeFile = "Encoded.txt";
		string decodeFile = "Decoed.txt";

		encode(textFile, encodeFile);

		decode(encodeFile, decodeFile);
	}

	//Exception handling.
	catch (DataError o) {
		cerr << o.getWhat() << "," << o.getWhere() << "," << o.getValue() << ")" << endl;
	}

	catch (bad_alloc) {
		cerr << "Error! Bad alloc!" << endl;
	}

	catch (bad_cast) {
		cerr << "Error! Bad cast!" << endl;
	}
}