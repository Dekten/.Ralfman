//HUFFMAN CODE
#include <iostream>
using namespace std;

void encode(string ifile, string ofile);

int main() {
	setlocale(LC_ALL, "rus"); 

	string textFile = "EasyText.txt";
	string encodeFile = "Encoded.txt";

	encode(textFile, encodeFile);
}