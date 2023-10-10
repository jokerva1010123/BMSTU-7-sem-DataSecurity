#include <iostream>

#include "licence_checker.h"

using namespace std;

int main()
{
	LicenceChecker lc;
	int has_licence = lc.checkLicence();
	if (has_licence == 0)
	{
		cout << "Licence is ok" << endl;
		cout << "Hello World!" << endl;
	}
	else if (has_licence == -1)
	{
		cout <<  "No licence" << endl;
	}
	else if (has_licence == 1)
	{
		cout << "Cannot check licence" << endl;
	}
	return 0;
}
