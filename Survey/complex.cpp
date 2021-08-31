#include "complex.h"

const int ASKII_difference = 48;

complex::complex(void) // default constructor
{
	Real = 0;
	Imaginary = 0;
} 
complex::complex(int R, int I) // constructor
{
	Real = R;
	Imaginary = I;
}
complex::~complex(void) // destructor
{
}
complex complex::operator+ (const complex &x) const // operator of binary '+'
{
	complex result;
	result.Real = Real + x.Real;
	result.Imaginary = Imaginary + x.Imaginary;
	return result;
}
complex complex::operator- () const // operator of '-'
{
	return complex(-Real, -Imaginary);
}
complex complex::operator- (const complex &x) const // operator of binary '-'
{
	return *this + (-x);
}
bool complex::operator== (const complex &x) const // operator of logical '=='
{
	return (Real == x.Real) & (Imaginary == x.Imaginary);
}
complex complex::operator= (const complex &x) // operator of '='
{
	Real = x.Real;
	Imaginary = x.Imaginary;
	return x;
}
std::ostream& operator<< (std::ostream &os, const complex &x) // overload for ostream <<
{
	char sign = '+';
	if (x.Imaginary < 0) sign = '-';
	os << x.Real << " " << sign << " " << abs(x.Imaginary) << "i";
	return os;
}
std::istream& operator>> (std::istream &is, complex &x) // overload for istream >>
{
	int Real = 0;
	int Imaginary = 0;
	char c = 0;
	int sign1 = 1, sign2 = 0;
	is.get(c);
	if (c == '-') // checking the '+'/'-'
	{ 
		sign1 = -1;
		is.get(c);
	}
	if (!(is_number(c) || c == '+')) return is;
	for (int i(0); is_number(c); i++) // Reading the real part
	{
		Real = 10 * Real + c - ASKII_difference;
		is.get(c);
	}
	if (c != ' ') return is; // checking the first 'space'
	else is.get(c);
	if (!(c == '+' || c == '-')) return is; // checking the '+'/'-'
	else
	{
		if (c == '+') sign2 = 1;
		else sign2 = -1;
		is.get(c);
	}
	if (c != ' ') return is; // checking the 2-nd 'space'
	else is.get(c);
	for (int i(0); is_number(c); i++) // Reading the Imaginary part
	{
		Imaginary = 10 * Imaginary + c - ASKII_difference;
		is.get(c);
	}
	if (c != 'i') return is; // checking the 'i' 
	else is.get(c); 
	if (c != '\n') return is; // checking the end of the string input
	else // finally the end - parsing Real and Imaginary into our complex
	{
		x.Real = sign1*Real;
		x.Imaginary = sign2*Imaginary;
		std::cout << "input done" << std::endl;
	}
	return is;
}
bool is_number(char i) // addditional function to clarify if the char is a number
{
	if (i == '0' || i == '1' || i == '2' || i == '3' || i == '4' 
	|| i == '5' || i == '6' || i == '7' || i == '8' || i == '9') return true;
	else return false;
}