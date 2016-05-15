#include <iostream>
#include <exception>
using namespace std;
#pragma once

class NotFoundFile : public exception {
public:
	//Information about error.
	virtual const char* what() const throw() {
		return "Error! File not found!";
	}

	NotFoundFile();

	virtual ~NotFoundFile();
};;

