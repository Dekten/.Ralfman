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
		//���������� ������������
		while (!F.eof())
		{
			char current;
			F >> current;
			character[unsigned char(current)]++;
			symbols++;
		}
		F.close();

		cout << "���������� ��������: " << symbols << endl;
		for (int i = 0; i < 256; i++) {
			if (character[i] != 0)
				cout << char(i) << ": " << character[i] << endl;

		}

		//������ �� ���� �������������� ��������
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

		//���������� ������
		Tree* tree2;
		Tree* tree3;
		do
		{
			tree = list.deleteHead();
			if (!list.isEmpty())
			{
				tree2 = list.deleteHead();
				tree3 = new Tree;
				//tree3 ������� �������
				//tree3 ����� ��������� - tree
				//tree3 ������ - tree2
				//�������� tree3 � list
			}
		} while (!list.isEmpty()); 


	}
	else cout << "���� �� ����������!" << endl;
}