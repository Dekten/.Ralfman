#include <fstream>
#include <iostream>
using namespace std;
#include <string>
#include <fcntl.h>
#include "List.h"
#include "Tree.h"

typedef unsigned char byte;
void turn(bool bits[8])
{
	for (int i = 0; i < 4; i++) {
		bool temp = bits[i];
		bits[i] = bits[7 - i];
		bits[7 - i] = temp;
	}
}
byte pack_byte(bool bits[8])
{
	byte result(0);
	for (unsigned i(8); i--;)
	{
		result <<= 1;
		result |= byte(bits[i]);
	}
	return result;
}

void unpack_byte(byte b, bool bits[8]) {
	for (unsigned i(0); i < 8; i++) {
		bits[i] = (b & 0x01) != 0;
		b >>= 1;
	}
}

void write(unsigned char& symbol, ofstream& E, char& position, bool* mes)
{
	mes[position] = (symbol == 0) ? 0 : 1;
	if (position == 7)
	{
		turn(mes);
		E.put(pack_byte(mes));
		position = 0;
	}
	else position++;
}

void writelast(ofstream& E, char& position, bool* mes)
{
	for (int i = position + 1; i < 8; i++) {
		mes[i] = 0;
	}
	E.put(pack_byte(mes));
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

		//-----------Cписок из всех использованных символов-----------------------------------------
		List<Tree> list;
		Tree* tree;
		ofstream T;
		T.open("Table.txt");

		for (int i = 0; i < 256; ++i)
		{
			if (character[i] != 0)
			{
				tree = new Tree;
				/*cout << unsigned char(i) << " " << character[i] << endl;*/
				tree->root_->frequency_ = character[i];
				tree->root_->symbol_ = char(i);
				list.insert(*tree);
				T << i << " " << character[i] << endl;
			}
		}
		T.close();

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
		}

		//----------------------------Запись в файл Encoded--------------------------------------------------
		unsigned char ch2;
		char position = 0;
		bool mes[8] = { 0 };

		while (F.get(current))
		{
			for (unsigned int i = 0; i < table[unsigned char(current)].size(); ++i)
			{
				if (table[unsigned char(current)].at(i) == '0')
					ch2 = 0;
				if (table[unsigned char(current)].at(i) == '1')
					ch2 = 1;
				write(ch2, E, position, mes);
			}
		}
		//writelast(E, position, mes);

		F.close();
		E.close();
	}
	else cout << "Файл не существует!" << endl;
}

void decode(string ifile, string ofile) {
	ifstream F;
	F.open(ifile);
	if (F)
	{
		//-------------------Создание массива из символов, ассоциированных с их частотой------------------------
		ifstream T;
		T.open("Table.txt");
		int arrSymbols[256] = { 0 };
		int i = 0;
		int tempFrequency = 0;

		while (!T.eof()) {
			if (!T.eof()) {
				T >> i;
				T >> tempFrequency;
				arrSymbols[i] = tempFrequency;
			}
		}

		T.close();

		//-------------------Восстановление дерева Хаффмана по массиву из символов с их частотой-----------------
		Tree* huffTree;
		List<Tree> tempList;
		for (i = 0; i < 256; i++) {
			if (arrSymbols[i] != 0) {
				huffTree = new Tree;
				/*cout << unsigned char(i) << " " << arrSymbols[i] << endl;*/
				huffTree->root_->frequency_ = arrSymbols[i];
				huffTree->root_->symbol_ = char(i);
				tempList.insert(*huffTree);
			}
		}

		Tree* subTree1;
		Tree* subTree2;
		do
		{
			huffTree = tempList.deleteHead();
			if (!tempList.isEmpty())
			{
				subTree1 = tempList.deleteHead();
				subTree2 = new Tree;
				subTree2->root_->frequency_ = subTree1->root_->frequency_ + huffTree->root_->frequency_;
				subTree2->root_->left_ = huffTree->root_;
				subTree2->root_->right_ = subTree1->root_;
				tempList.insert(*subTree2);
			}
		} while (!tempList.isEmpty());

		//--------------------Считывние закодированного файла, его раскодировка и запись в файл----------------------------
		ofstream D;
		D.open(ofile);

		char currentSymbol;
		bool mes[8] = { 0 };
		i = 0;
		Tree::Node* root = huffTree->root_;

		while (!F.eof()) {
			F.get(currentSymbol);
			/*cout << currentSymbol << endl;*/
			unpack_byte(currentSymbol, mes);
			turn(mes);
			/*for (i = 0; i < 8; i++)
				cout << mes[i];
			cout << endl;*/
			for (i = 0; i < 8; i++) {
				if (!mes[i])
					root = root->left_;
				else root = root->right_;
				if (root->symbol_ != NULL) {
					D << root->symbol_;
					root = huffTree->root_;
				}
			}
		}
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

////Функция для построения нового пустого дерева на основе количества элементов.
//void Tree::createNewTree(Node* root, int kolElements) const
//{
//	if (kolElements == 0)
//		root = NULL;
//	else {
//		int kolLeft = kolElements / 2;
//		int kolRight = kolElements - kolLeft - 1;
//		Node* temp = new Node;
//		createNewTree(temp->left_, kolLeft);
//		createNewTree(temp->right_, kolRight);
//		root = temp;
//	}
//}
//
////Функция для добавления символа в созданное пустое дерево.
//void Tree::addSymbols(Node* root, string code) const
//{
//	//начинаем смотреть строку со второго символа.
//	static int i = 0;
//	i++;
//	if (i < code.length()) {
//		if (code[i] == '0')
//			addSymbols(root->left_, code);
//		if (code[i] == '1')
//			addSymbols(root->right_, code);
//	}
//	//вставляем в данный лист символ из строки, стоящий на первом месте.
//	root->symbol_ = code[0];
//}

//	ifstream F;
//	F.open(ifile);
//	if (F) 
//	{
//		//-----------------Открытие файла Table.txt------------------------------
//		ifstream T;
//		T.open("Table.txt");
//
//		//-----------------Построение нового дерева------------------------------
//		int maxHeight = 10; //Максимальная высота дерева, записанная в каком-нибудь файле, временно = 10.
//		//Определение количества элементов дерева на основе полученной высоты.
//		int k = 2;
//		int maxKol = 1;
//		for (int i = 1; i <= maxHeight; i++) {
//			maxKol += k;
//			k *= 2;
//		}
//		//Создание нового дерева на основе количества элементов в этом дереве.
//		Tree* huffTree = new Tree;
//		huffTree->createNewTree(huffTree->root_, maxKol - 1);
//
//		//-----------------Чтение и анализ файла Table.txt-------------------------
//		string temp;
//		while (!T.eof())
//		{
//			T >> temp;
//			huffTree->addSymbols(huffTree->root_, temp);
//		}
//		
//	}
//	else cout << "Файл не существует!" << endl;