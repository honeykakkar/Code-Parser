////////////////////////////////////////////////////////////////////////
// ASTNode.cpp - package to test nodes or element of AST             //                                                        
// Language:    C++, Visual Studio 2015                            ///////
// Application: Code Parser with AST, CSE687 - Object Oriented Design  ////////
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
*ASTNode.cpp - package to test nodes or element of AST
*
* Build Process :
*--------------
* Required Files : ASTNode.h
* Build Command : devenv Parser.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 16 2016
*/

#include "../Parser/ASTNode.h"
#include <iostream>
using namespace Scanner;

void DisplayHeader()
{
	std::cout << "  Testing ASTNode or element.....";
}

// Test package for ASTNode or element
#ifdef TEST_NODE

int main()
{
	element Root;
	Root.name = "Root";
	Root.type = "Node";
	Root.startLineCount = 1;
	Root.endLineCount = 0;

	element N1;
	N1.name = "Node 1";
	N1.type = "Node";
	N1.startLineCount = 1;
	N1.endLineCount = 0;
	Root.GetChildren().push_back(&N1);

	element N2;
	N2.name = "Node 2";
	N2.type = "Node";
	N2.startLineCount = 1;
	N2.endLineCount = 0;
	N1.GetChildren().push_back(&N2);


	element N3;
	N3.name = "Node 3";
	N3.type = "Node";
	N3.startLineCount = 1;
	N3.endLineCount = 0;
	Root.GetChildren().push_back(&N3);

	Root.show();
	N1.show();
	N2.show();
	N3.show();
	return 0;
}
#endif // TEST_NODE