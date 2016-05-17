//myException.h 
#pragma once 
#include <iostream> 
using namespace std;

class myException
{
protected:
	string what_;
	string where_;
	int value_;
public:
	myException() {};
	~myException() {};
	string getWhat() { return what_; }
	string getWhere() { return where_; }
	int getValue() { return value_; }
};

class DataError : public myException
{
public:
	DataError(const string what, const string where, const int n) { what_ = what; where_ = where; value_ = n; }
};
