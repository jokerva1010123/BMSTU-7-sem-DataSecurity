#include <iostream>
#include <string>
#include <fstream>

#include "installer.h"

using namespace std;

int main()
{
	Installer inst;
	return inst.install("licence.key");
}
