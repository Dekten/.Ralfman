#include <fstream>
#include <iostream>
using namespace std;
#include <string>
#include "List.h"
#include "Tree.h"

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
		//Вычисление частотностей
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

		//список из всех использованных символов
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

		//построение дерева по списку
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

		//Создание таблицы кодов
		////Создание таблицы ассоциаций символа и его кода.
		//Associations* table = new Associations[kolSymbols];
		////Создание ассоциаций.
		//createAssoсiations(tree->root_, table, kolSymbols);
		F.clear();
		F.seekg(0);
		ofstream E;
		E.open(ofile);

		string table[256];
		unsigned char symbol;
		for (int number = 0; number < 256; ++number) {
			table[number] = "";
			symbol = unsigned char(number);
			tree->build(tree->root_, symbol, "", table[number]);
			//cout << char(number) << table[number] << endl;
			if (table[number] != "")
				E << unsigned char(number) << table[number] << endl;
		}

		//Запись в файл Encoded
		while (!F.eof())
		{
			F.get(current);
			E << table[unsigned char(current)];
		}

		F.close();
		E.close();
	}
	else cout << "Файл не существует!" << endl;
}

void decode(string ifile, string ofile) {
	ifstream F;
	F.open(ifile);
	F.close();
}

void Tree::build(Node* root, unsigned char symbol, string temp, string & code) const
{
	if (root) //if the node is not null
	{
		//compare char of the leaf node and the given char
		if (!root->left_ && !root->right_ && root->symbol_ == symbol)
		{
			code = temp; //if the char is found then copy the H. string
		}
		else
		{
			//continue to search if the same char still not found
			build(root->left_, symbol, temp + "0", code);
			build(root->right_, symbol, temp + "1", code);
		}
	}
}
