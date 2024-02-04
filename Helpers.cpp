#include "Helpers.h"
#include <ctype.h>
#include <iostream>

void CapFirstLetter(char* name)
{
	//std::cout << "Original char* " << name << '\n';
	name[0] = toupper(name[0]);
	//std::cout << "Modified char* " << name << '\n';
}