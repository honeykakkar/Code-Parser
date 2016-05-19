// MetricsAnalyzer.cpp : Defines the entry point for the console application.
///////////////////////////////////////////////////////////////////////////////
// MetricsAnalyzer.cpp - package for evaluating and displaying the size      //
//                     and complexity of all global functions, static        //
//                     member functions, and non-static member functions     //
//                     in each of a set of specified packages                //
// Language:    C++, Visual Studio 2015                                      //
// Application: Code Parser with AST, CSE687 - Object Oriented Design        //
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
*AbstractSyntaxTree.cpp: Contains the code to package for evaluating and displaying
*  the size and complexity of all global functions, static member functions, and
*   non-static member functions in each of a set of specified packages
*
* Build Process :
*--------------
* Required Files : ASTNode.h, MetricsAnalyzer.h, Tokenizer.h, SemiExp.h, Parser.h, ActionsAndRules.h
*					ConfigureParser.h, FileMgr.h
* Build Command : devenv ASTree.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 16 2016
*/
#include "MetricsAnalyzer.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../FileMgr/FileMgr.h"
#include <iostream>

using namespace Scanner;

// Method to analyze file and output only function scopes. 
// Bool parameters decide whether to print AST for the file and whether to print complexity table
void MetricsAnalyzer::FullMetricAnalyzerforFile(std::string File, bool DisplayTree, bool DisplayTable)
{
	std::string fileSpec = Scanner::Path::getFullFileSpec(File);
	std::cout << "  Analyzing file: " << fileSpec << "\n";
	Scanner::ConfigParseToConsole configure;
	Parser* pParser = configure.Build();  // Adding rules and actions to the parser
	try
	{
		if (pParser)
		{
			if (!configure.Attach(fileSpec))
				std::cout << "\n  Could not open file " << fileSpec << std::endl;
		}
		else
			std::cout << "\n\n  Parser not built\n\n";

		while (pParser->next())
			pParser->parse();
		std::cout << "\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	if (DisplayTable)
		configure.FullAnalyze();  // calling method to analyze the file by running through its AST and building complexity table
	if (DisplayTree)
		configure.DisplayFullAST(); // method to display AST if asked for
}


//Method to analyze a particular file with given name in the directory. It analyzes each scope in each file
// Bool parameters decide whether to print AST for the file and whether to print complexity table
void MetricsAnalyzer::GranularMetricAnalyzerforFile(std::string File, bool DisplayTree, bool DisplayTable)
{
	std::string fileSpec = Scanner::Path::getFullFileSpec(File);
	std::cout << "  Analyzing file: " << fileSpec << "\n";
	Scanner::ConfigParseToConsole configure;
	Parser* pParser = configure.Build();  // Adding rules and actions to the parser
	try
	{
		if (pParser)
		{
			if (!configure.Attach(fileSpec))
				std::cout << "\n  Could not open file " << fileSpec << std::endl;
		}
		else
			std::cout << "\n\n  Parser not built\n\n";

		while (pParser->next())
			pParser->parse();
		std::cout << "\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	if (DisplayTable)
		configure.GranularAnalyze();  // calling method to analyze the file by running through its AST and building complexity table
	if (DisplayTree)
		configure.DisplayGranularAST(); // method to display AST if asked for
}


// Test stub for MetricAnalyzer
// Make projects it depends on as static libraries in order to execute test stub. Make current project as an application.

#ifdef TEST_ANALYZERS
int main()
{
	MetricsAnalyzer MA;
	MA.GranularMetricAnalyzerforFile("../TestFiles/TestParser.cpp", true, true);
	MA.GranularMetricAnalyzerforFile("../TestFiles/TestParser.cpp", true, true);
	return 0;
}

#endif