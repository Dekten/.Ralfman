//HUFFMAN CODE
#include <iostream>
using namespace std;

void encode(string ifile, string ofile);

void decode(string ifile, string ofile);

int main() {
	setlocale(LC_ALL, "rus"); 

	string textFile = "Text.txt";
	string encodeFile = "Encoded.txt";
	string decodeFile = "Decoded.txt";

	encode(textFile, encodeFile);

	/*decode(encodeFile, decodeFile);*/

	return 0;
}