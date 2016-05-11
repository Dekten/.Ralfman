#pragma once

class Tree
{
protected:
	struct Node {
		unsigned char symbol_;
		int frequency_;
		Node* left_;
		Node* right_;
		Node(char symbol = 0, int frequency = 0, Node* left = 0, Node* right = 0) {
			frequency_ = frequency;
			symbol_ = symbol;
			left_ = left;
			right_ = right;
		}
	};
	Node* root_;
private:
	//int countSubTree(Node* root_);
	void LARSubTree(Node* root_, string* table, ofstream& T);
public:

	Tree() {
		Node* root = new Node;
		root_ = root;
	};
	~Tree() {};

	//int count();
	void LAR(string* table, ofstream& T);
	void setFrequency(int);
	void setChar(char);
	int getFrequency();
	char getChar();

	bool operator==(const Tree &) const;
	bool operator!=(const Tree &) const;
	bool operator<(const Tree &) const;
	bool operator>(const Tree &) const;
	bool operator<=(const Tree &) const;
	bool operator>=(const Tree &) const;

	friend void encode(string ifile, string ofile);
	void build(Node* root, unsigned char symbol, string temp, string & code) const;
};

//template<class T>
//int Tree<T>::countSubTree(Node* root_)
//{
//	if (root_ == 0) return 0;
//	return countSubTree(root_->left_) + 1 + countSubTree(root_->right_);
//}
//
//template<class T>
//int Tree<T>::count()
//{
//	return countSubTree(root_);
//}

void Tree::LARSubTree(Node* root, string* table, ofstream& T)
{
	if (root != 0) {
		LARSubTree(root->left_, table, T);
		if (root->symbol_)
			T << root->symbol_ << table[unsigned char(root->symbol_)] << endl;
		//if (!root_->symbol_)
		//	T << root_->frequency_ << " ";
		LARSubTree(root->right_, table, T);
	}
}

void Tree::LAR(string* table, ofstream& T)
{
	return LARSubTree(root_, table, T);
}


bool Tree::operator==(const Tree & T) const
{
	return (root_->frequency_ == T.root_->frequency_);
}

bool Tree::operator!=(const Tree & T) const
{
	return (root_->frequency_ != T.root_->frequency_);
}

bool Tree::operator<(const Tree & T) const
{
	return (root_->frequency_ < T.root_->frequency_);
}

bool Tree::operator>(const Tree & T) const
{
	return (root_->frequency_ > T.root_->frequency_);
}

bool Tree::operator<=(const Tree & T) const
{
	return (root_->frequency_ <= T.root_->frequency_);
}

bool Tree::operator>=(const Tree & T) const
{
	return (root_->frequency_ >= T.root_->frequency_);
}

void Tree::setFrequency(int f)
{
	root_->frequency_ = f;
}

void Tree::setChar(char c)
{
	root_->symbol_ = c;
}

int Tree::getFrequency()
{
	return root_->frequency_;
}

char Tree::getChar()
{
	return root_->symbol_;
}

