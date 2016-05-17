//List.h
#pragma once

template<class TableFile>
class List
{
private:
	struct Node {
		TableFile inf_;
		Node* next_;
		Node(TableFile inf, Node* next = 0) {
			inf_ = inf;
			next_ = next;
		}
	};
	Node* head_;
	Node* tail_;

public:
	List() { head_ = tail_ = 0; }
	~List();
	List(const List& ob);


	TableFile head() { return head_->inf_; }
	TableFile tail() { return tail_->inf_; }
	bool isEmpty() { return head_ == 0; }
	void addHead(const TableFile& ob);
	void addTail(const TableFile& ob);
	void insert(const TableFile& ob);
	TableFile* deleteHead();
};

template<class TableFile>
List<TableFile>::~List()
{
	while (head_ != 0) {
		Node *next = head_->next_;
		delete head_;
		head_ = next;
	}
}

template<class TableFile>
List<TableFile>::List(const List& ob)
{
	Node* current = ob->head_;
	while (current != 0) {
		this->addTail(current->inf_);
		current = current->next_;
	}
}

template<class TableFile>
void List<TableFile>::addHead(const TableFile& ob)
{
	Node* pHead = head_;
	Node* node = new Node(ob);

	head_ = node;
	head_->next_ = pHead;
	if (pHead == 0)
		tail_ = head_;
}


template<class TableFile>
void List<TableFile>::addTail(const TableFile& ob)
{
	Node* node = new Node(ob);
	if (tail_ == 0)
		head_ = node;

	if (tail_ == 0)
		tail_ = node;
	else {
		tail_->next_ = node;
		tail_ = node;
	}
}

template<class TableFile>
void List<TableFile>::insert(const TableFile& ob)
{
	Node* current = head_;
	Node* previous = 0;
	if (head_ == 0) {
		this->addHead(ob);
		return;
	}

	while ((current->inf_ < ob) && (current->next_ != 0)) {
		previous = current;
		current = current->next_;
	}

	if ((current->inf_ >= ob) && (previous == 0))
		this->addHead(ob);
	else if ((current->inf_ <= ob) && (current->next_ == 0))
		this->addTail(ob);
	else {
		Node* newNode = new Node(ob);
		previous->next_ = newNode;
		newNode->next_ = current;
	}
}

template<class TableFile>
TableFile* List<TableFile>::deleteHead()
{
	if (head_ != 0) {
		TableFile* pHead = &head_->inf_;
		head_ = head_->next_;
		return pHead;
	}
	else throw DataError("List is empty!", "deleteHead", 0);
}






