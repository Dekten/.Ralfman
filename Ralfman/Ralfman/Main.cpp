//Main.cpp
#include <string>
#include <iostream>
using namespace std;

#include "myException.h"

void encode(string ifile, string ofile);

void decode(string ifile, string ofile);

int main() {
	setlocale(LC_ALL, "rus");

	try {
		string textFile	  = "Text.txt";
		string encodeFile = "Encoded.txt";
		string decodeFile = "Decoded.txt";

		encode(textFile, encodeFile);

		decode(encodeFile, decodeFile);
	}

	//Exception handling.
	catch (DataError o) 
	{
		cerr << "ERROR: " << o.getWhat() << "," << o.getWhere() << "," << o.getValue() << ")" << endl;
	}

	catch (bad_alloc) 
	{
		cerr << "Error! Bad alloc!" << endl;
	}

	catch (bad_cast) 
	{
		cerr << "Error! Bad cast!" << endl;
	}
}