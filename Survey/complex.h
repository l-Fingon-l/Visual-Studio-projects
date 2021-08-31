#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

class complex
{
private:
	int Real; // Real part
	int Imaginary; // Imaginary part
public:
	complex(void); // default constructor
	complex(int Real, int Imaginary); // constructor

	complex operator + (const complex &x) const; // operator of binary '+'
	complex operator - ()const; // operator of '-'
	complex operator - (const complex &x) const; // operator of binary '-'
	bool operator == (const complex &x) const; // operator of logical '=='
	complex operator = (const complex &x); // operator of '='
	friend std::ostream& operator<< (std::ostream &os, const complex &x); // overload for ostream <<
	friend std::istream& operator>> (std::istream &is, complex &x); // overload for istream >>

	~complex(void); // destructor
};

bool is_number(char i); // addditional function to clarify if the char is a number