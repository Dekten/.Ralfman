#include <fstream>
#include <iostream>
using namespace std;
#include <string>
#include <fcntl.h>
#include "List.h"
#include "Tree.h"

void write(unsigned char symbol, ofstream& E)
{
	static int position = 0;
	static unsigned char byte = '\0';

	if (symbol<2) //if not EOF
	{
		if (symbol == 1)
			byte = byte | (symbol << (7 - position));
		else //symbol==0
			byte = byte & static_cast<unsigned char>(255 - (1 << (7 - position)));
		++position;
		position %= 8;
		if (position == 0)
		{
			E.put(byte);
			byte = '\0';
		}
	}
	else
	{
		E.put(byte);
	}
}


//struct Associations {
//	List<bool> code;
//	char symbol;
//};
//
////Рекурсивная функция для создания ассоциаций.
//void createAssoсiations(Tree::Node* root, Associations* table, int kolSymbols) {
//	int i = 0;
//	if (root->left_ != NULL) {
//		table[i].code.addTail(0);
//		createAssoсiations(root->left_, table, kolSymbols);
//	}
//	if (root->right_ != NULL) {
//		table[i].code.addTail(1);
//		createAssoсiations(root->right_, table, kolSymbols);
//	}
//	if (root->symbol_) {
//		table[i].symbol = root->symbol_;
//		i++;
//	}
//
//	//Для следующего символа в поле code добавляем код предыдущего без последней цифры.
//	if (i < kolSymbols) {
//		table[i].code = table[i - 1].code;
//		//Удалить хвостовой элемент из списка code??
//	}
//}

void encode(string ifile, string ofile)
{
	int kolSymbols = 0;
	int character[256] = { 0 };
	ifstream F;
	F.open(ifile);
	if (F)
	{
		//-----------Вычисление частотностей-----------------------------------------------
		char current;
		F.get(current);
		while (!F.eof())
		{
			character[unsigned char(current)]++;
			kolSymbols++;
			F.get(current);
		}

		cout << "Количество символов: " << kolSymbols << endl;
		for (int i = 0; i < 256; i++) {
			if (character[i] != 0)
				cout << char(i) << ": " << character[i] << endl;

		}

		//-----------Cписок из всех использованных символов-----------------------------------------
		List<Tree> list;
		Tree* tree;

		for (int i = 0; i < 256; ++i)
		{
			if (character[i] != 0)
			{
				tree = new Tree;
				tree->root_->frequency_ = character[i];
				tree->root_->symbol_ = char(i);
				list.insert(*tree);
			}
		}

		//------------------------Построение дерева по списку-------------------------------------------
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

		//---------------------Создание таблицы кодов---------------------------------------------
		////Создание таблицы ассоциаций символа и его кода.
		//Associations* table = new Associations[kolSymbols];
		////Создание ассоциаций.
		//createAssoсiations(tree->root_, table, kolSymbols);
		F.clear();
		F.seekg(0);
		ofstream E;
		ofstream T;
		E.open(ofile);
		T.open("Table.txt");

		string table[256];
		unsigned char symbol;
		for (int number = 0; number < 256; ++number) {
			table[number] = "";
			symbol = unsigned char(number);
			tree->build(tree->root_, symbol, "", table[number]);
			//cout << char(number) << table[number] << endl;
			//if (table[number] != "")
			//	T << unsigned char(number) << table[number] << endl;
		}
		//
		tree->LAR(table, T);
		T.close();

		//----------------------------Запись в файл Encoded--------------------------------------------------
		unsigned char ch2;
		while (F.get(current))
		{
			for (unsigned int i = 0; i<table[unsigned char(current)].size(); ++i)
			{
				if (table[unsigned char(current)].at(i) == '0')
					ch2 = 0;
				if (table[unsigned char(current)].at(i) == '1')
					ch2 = 1;
				write(ch2, E);
			}
		}

		F.close();
		E.close();
	}
	else cout << "Файл не существует!" << endl;
}

//Строит код символа по дереву Хаффмана
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