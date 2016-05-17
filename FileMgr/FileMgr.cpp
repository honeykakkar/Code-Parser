/////////////////////////////////////////////////////////////////////
// FileMgr.cpp - find files matching specified patterns            //
//               on a specified path                               //
// ver 2.0                                                         //
// Jim Fawcett, Help Session Demo, Spring 2015                     //
/////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include "../DataStore/DataStore.h"
#include <iostream>
//-----------------------------<Test Stub>--------------

using namespace Scanner;

void Display()
{
	std::cout << "  Testing FileManagers.....";
}
#ifdef TEST_FILEMGRS
int main()
{
	std::cout << "\n  Testing FileMgr";
	std::cout << "\n =================";

	DataStore ds;
	FileMgr fm("..", ds);
	//fm.addPattern("*.h");
	fm.addPattern("*.cpp");
	//fm.addPattern("*.partial");
	fm.search();
	//std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------\n";
	for (auto fs : ds)
	{
		std::cout << " " << fs << std::endl;
	}
	std::cout << "\n\n";
	return 0;
}
#endif
