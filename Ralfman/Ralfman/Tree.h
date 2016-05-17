//Tree.h
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

public:

	Tree() {
		Node* root = new Node;
		root_ = root;
	};
	~Tree() {};

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
	friend void decode(string ifile, string ofile);
	friend void HuffmanTreeBuild(Tree* & HuffmanTree, List<Tree> & tempList);

	void buildCode(Node* root, unsigned char symbol, string temp, string & code) const;
};

bool Tree::operator==(const Tree & TableFile) const
{
	return (root_->frequency_ == TableFile.root_->frequency_);
}

bool Tree::operator!=(const Tree & TableFile) const
{
	return (root_->frequency_ != TableFile.root_->frequency_);
}

bool Tree::operator<(const Tree & TableFile) const
{
	return (root_->frequency_ < TableFile.root_->frequency_);
}

bool Tree::operator>(const Tree & TableFile) const
{
	return (root_->frequency_ > TableFile.root_->frequency_);
}

bool Tree::operator<=(const Tree & TableFile) const
{
	return (root_->frequency_ <= TableFile.root_->frequency_);
}

bool Tree::operator>=(const Tree & TableFile) const
{
	return (root_->frequency_ >= TableFile.root_->frequency_);
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

