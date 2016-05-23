//List.h
#pragma once

template<class Type>
class List
{
private:
	struct Node {
		Type inf_;
		Node* next_;
		Node(Type inf, Node* next = 0) {
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


	Type head() { return head_->inf_; }
	Type tail() { return tail_->inf_; }
	bool isEmpty() { return head_ == 0; }
	void addHead(const Type& ob);
	void addTail(const Type& ob);
	void insert(const Type& ob);
	Type* deleteHead();
};

template<class Type>
List<Type>::~List()
{
	while (head_ != 0) {
		Node *next = head_->next_;
		delete head_;
		head_ = next;
	}
}

template<class Type>
List<Type>::List(const List& ob)
{
	Node* current = ob->head_;
	while (current != 0) {
		this->addTail(current->inf_);
		current = current->next_;
	}
}

template<class Type>
void List<Type>::addHead(const Type& ob)
{
	Node* pHead = head_;
	Node* node = new Node(ob);

	head_ = node;
	head_->next_ = pHead;
	if (pHead == 0)
		tail_ = head_;
}


template<class Type>
void List<Type>::addTail(const Type& ob)
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

template<class Type>
void List<Type>::insert(const Type& ob)
{
	Node* current  = head_;
	Node* previous = 0;
	if (head_ == 0) {
		this->addHead(ob);
		return;
	}

	while ((current->inf_ < ob) && (current->next_ != 0)) {
		previous = current;
		current = current->next_;
	}

	if		((current->inf_ >= ob) && (previous == 0))
		this->addHead(ob);
	else if ((current->inf_ <= ob) && (current->next_ == 0))
		this->addTail(ob);
	else {
		Node* newNode = new Node(ob);
		previous->next_ = newNode;
		newNode->next_ = current;
	}
}

template<class Type>
Type* List<Type>::deleteHead()
{
	if (head_ != 0) {
		Type* pHead = &head_->inf_;
		head_ = head_->next_;
		return pHead;
	}
	else throw DataError("List is empty!", "deleteHead", 0);
}






