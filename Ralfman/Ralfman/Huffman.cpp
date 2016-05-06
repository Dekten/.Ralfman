#include <fstream>
#include <iostream>
using namespace std;
#include <string>
#include "List.h"
#include "Tree.h"

void encode(string ifile, string ofile)
{
	int symbols = 0;
	int character[256] = { 0 };
	fstream F;
	F.open(ifile);
	if (F)
	{
		//Вычисление частотностей
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

		//список из всех использованных символов
		List<Tree> list;
		Tree* tree;

		for (int i = 0; i<256; ++i)
		{
			if (character[i] != 0)
			{
				tree = new Tree;
				tree->setFrequency(character[i]);
				tree->setChar(char(i));
				list.insert(*tree);
			}
		}

		//построение дерева
		Tree* tree2;
		Tree* tree3;
		do
		{
			tree = list.deleteHead();
			if (!list.isEmpty())
			{
				tree2 = list.deleteHead();
				tree3 = new Tree;
				//tree3 частоты сложить
				//tree3 левое поддерево - tree
				//tree3 правое - tree2
				//вставить tree3 в list
			}
		} while (!list.isEmpty()); 


	}
	else cout << "Файл не существует!" << endl;
}