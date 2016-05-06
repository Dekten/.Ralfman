#pragma once

class Tree
{
protected:
	struct Node {
		char symbol_;
		int frequency_;
		Node* left_;
		Node* right_;
		Node(char symbol,int frequency, Node* left = 0, Node* right = 0) {
			frequency_ = frequency;
			symbol_ = symbol;
			left_ = left;
			right_ = right;
		}
	};
	Node* root_;
private:
	//int countSubTree(Node* root_);
	//void LARSubTree(Node* root_);
public:
	Tree() :root_(0) {};
	~Tree() {};
	
	//int count();
	//void LAR();
	void setFrequency(int);
	void setChar(char);

	bool operator==(const Tree &) const;
	bool operator!=(const Tree &) const;
	bool operator<(const Tree &) const;
	bool operator>(const Tree &) const;
	bool operator<=(const Tree &) const;
	bool operator>=(const Tree &) const;
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




//template<class T>
//void Tree<T>::LARSubTree(Node* root_)
//{
//	if (root_ != 0) {
//		LARSubTree(root_->left_);
//		cout << root_->key_ << " ";
//		LARSubTree(root_->right_);
//	}
//
//	if (root_ == root_)	//Обеспечивает перенос на новую строку
//		cout << endl;
//}
//
//template<class T>
//void Tree<T>::LAR()
//{
//	return LARSubTree(root_);
//}


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

