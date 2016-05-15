#include <iostream>
#include <exception>
using namespace std;
#pragma once

class DataError : public exception {
public:
	//Information about error.
	virtual const char* what() const throw() {
		return "Error! The list is empty!";
	}

	DataError();

	virtual ~DataError();
};;
