// Project2Demonstration.cpp : Defines the entry point for the console application.
// It is entry point of demonstration of each Metric Analysis for each file matching the pattern mentioned in command arguments.

///////////////////////////////////////////////////////////////////////////////
// Project2Demonstration.cpp - this package is entry point of demonstration  //
//				of each Metric Analysis for each file matching the pattern   //
//				mentioned in command arguments. collecting metrics for all   //
//				the package                                                  //
// Language:    C++, Visual Studio 2015                                      //
// Application: Code Parser with AST, CSE687 - Object Oriented Design        //
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
*AbstractSyntaxTree.cpp: Contains the code to package for collecting metrics for
*   all the packages with names that match a specified pattern in a directory tree
*   rooted at a specified path.
*
* Build Process :
*--------------
* Required Files : ASTNode.h, MetricsAnalyzer.h, Tokenizer.h, SemiExp.h, Parser.h, ActionsAndRules.h
*					ConfigureParser.h, FileMgr.h, MetricsExecutive.h, AbstractSyntaxTree.h
* Build Command : devenv Project2Demonstration.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 19 2016
*/

#include <string>
#include <iostream>
#include "../MetricsExecutive/MetricsExecutive.h"
#include "../MetricsAnalyzer/MetricsAnalyzer.h"

using namespace Scanner;

void UnderLine()
{
	for (int i = 1; i<40; i++)
		std::cout << "--";
	std::cout << "\n";
}

//Method used to open and display any file in the directory
void OpenFile(std::string File)
{
	std::string Disp;
	std::ifstream DemoFile(File);
	if (DemoFile.is_open())
	{
		while (getline(DemoFile, Disp))
			std::cout << Disp << '\n';
	}
	DemoFile.close();
	DemoFile.clear();
}

//Method to demonstrate that R3 and R4 of Project 2 have been met
void DemoR3_4()
{
	std::cout << "\n  Demonstrating requirement #3 and #4\n";
	UnderLine();
	std::cout << "\n  Following demonstration uses Tokenizer and SemiExp packages to calculate function size and complexity using Parser package\n";
	std::cout << "  Parser has rules to be detected and actions associated with each rule\n\n";
	MetricsExecutive ME;
	OpenFile("../TestFiles/TestParser.cpp");
	ME.GetAnalyzer()->GranularMetricAnalyzerforFile("../TestFiles/TestParser.cpp", false, true);
}

//Method to demonstrate that R5 and R6 of Project 2 have been met
void DemoR5_6()
{
	std::cout << "\n  Demonstrating requirement #5 and #6\n";
	UnderLine();
	std::cout << "\n  Following demonstration uses AST package along with all other packages required to parse the above file\n";
	MetricsExecutive ME;
	ME.GetAnalyzer()->GranularMetricAnalyzerforFile("../TestFiles/TestParser.cpp", true, false);
}

// Methd to demonstrate that R7 of Project 2 has been met
void DemoR7(int argc, char* argv[])
{
	std::cout << "\n  Demonstrating requirement #7\n";
	UnderLine();
	std::cout << "\n  Following demonstration uses FileManager package for  navigates through a directory tree rooted at some specified path and returns names of all the files and/or directories matching a pattern.\n";
	MetricsExecutive ME;
	ME.Displayfiles(argc, argv);
}

//Method to demonstrate that R8 and R9 of Project 2 have been met
void DemoR8_9(int argc, char* argv[])
{
	std::cout << "\n  Demonstrating requirement #8 and #9\n";
	UnderLine();
	std::cout << "\n  Following demonstration uses MetricsAnalyzer package for evaluating and displaying the size and complexity of all functions in each of a set of specified packages.\n";
	std::cout << "  Thus collecting metrics for all packages with names that match a specified pattern in a directory tree rooted at a specified path\n\n";
	MetricsExecutive ME;
	ME.PerformFullAnalysis(argc, argv);
	std::cout << "\n  Testing Parser, Tokenizer, SemiExp and ASTree package at very granular level (by analyzing all scopes)\n";
	UnderLine(); std::cout << "\n";
	//Further test of analyzer and parser at fine grain level with one complex .cpp file
	ME.GetAnalyzer()->GranularMetricAnalyzerforFile("../TestFiles/MetricsExecutive.cpp", true, true);

	//Further test of analyzer and parser at fine grain level with one complex .h file
	ME.GetAnalyzer()->GranularMetricAnalyzerforFile("../TestFiles/DataStore.h", true, true);
}

int main(int argc, char* argv[])
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimension
	MoveWindow(console, r.left, r.top, 1000, 600, TRUE); ////MoveWindow(window_handle, x, y, width, height, redraw_window);
	DemoR3_4();
	DemoR5_6();
	DemoR7(argc, argv);
	DemoR8_9(argc, argv);
	std::cout << "\nPress ENTER to exit.........\n";
	std::cin.get();
    return 0;
}

