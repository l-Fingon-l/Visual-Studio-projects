#include <cstdlib>	
#include "complex.h"

using namespace std;

int main()
{
	complex a (6, 4);
	complex b(-8, -20);
	complex c;
	complex d = a;

	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "a + b = " << a + b << endl;
	cout << d << " is d" << endl;

	cout << "d (('Real' + 'Imaginary'i) format): ";
	cin >> d;
	cout << "Your d: " << d << endl;

	system("PAUSE");
	return 0;
}