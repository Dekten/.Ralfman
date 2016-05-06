#include <fstream>
#include <iostream>
using namespace std;
#include <string>

void encode(string ifile, string ofile)
{
	int symbols = 0;
	int character[256] = { 0 };
	fstream F;
	F.open(ifile);
	if (F)
	{
		while (!F.eof())
		{
			char current;
			F >> current;
			character[unsigned char(current)]++;
			symbols++;
		}
		F.close();

		cout << "Количество символов: " << symbols << endl;
		for (int i = 0; i < 256; i++) {
			if (character[i] != 0)
				cout << char(i) << ": " << character[i] << endl;
		}
	}
	else cout << "Файл не существует!" << endl;
}