//HUFFMAN CODE
#include <iostream>
using namespace std;

void encode(string ifile, string ofile);

void decode(string ifile, string ofile);

int main() {
	setlocale(LC_ALL, "rus"); 

	try {
		string textFile = "WarAndPiece.txt";
		string encodeFile = "Encoded.txt";
		string decodeFile = "Decoded.txt";

		encode(textFile, encodeFile);

		decode(encodeFile, decodeFile);
	}

	//Exception handling.
	catch (const char* errStr) {
		cerr << "Error! " << errStr << endl;
	}

	catch (exception & err) {
		cerr << err.what() << endl;
	}

	catch (bad_alloc) {
		cerr << "Error! Bad alloc!" << endl;
	}

	catch (bad_cast) {
		cerr << "Error! Bad cast!" << endl;
	}

	return 0;
}