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
		char current;
		F.get(current);
		while (!F.eof())
		{
			character[unsigned char(current)]++;
			symbols++;
			F.get(current);
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
				tree->root_->frequency_ = character[i];
				tree->root_->symbol_ = char(i);
				list.insert(*tree);
			}
		}

		//���������� ������ �� ������
		Tree* tree2;
		Tree* tree3;
		do
		{
			tree = list.deleteHead();
			if (!list.isEmpty())
			{
				tree2 = list.deleteHead();
				tree3 = new Tree;
				//tree3->setFrequency(tree->getFrequency() + tree2->getFrequency());
				tree3->root_->frequency_ = tree2->root_->frequency_ + tree->root_->frequency_;
				tree3->root_->left_ = tree->root_;
				tree3->root_->right_ = tree2->root_;
				list.insert(*tree3);
			}
		} while (!list.isEmpty()); 

		//������ � ���� Encoded
	}
	else cout << "���� �� ����������!" << endl;
}