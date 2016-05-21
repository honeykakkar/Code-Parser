
///////////////////////////////////////////////////////////////////////////////
// MetricsExecutive.cpp - package for collecting metrics for all the packages //
//      with names that match a specified pattern in a directory tree rooted //
//      at a specified path                                                  //
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
*					ConfigureParser.h, FileMgr.h, MetricsExecutive.h
* Build Command : devenv ASTree.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 16 2016
*/
#include "MetricsExecutive.h"

using namespace Scanner;

// Method to send each file matching the pattern in a given directory to be analyzed for all scopes
void MetricsExecutive::FindAndAnalyzeFiles(std::string Path, std::string Pattern)
{
	std::cout << "\n  Metrics Executive - Analyzing "<< Pattern <<" files at path: " << Path << "\n\n";
	DataStore DStore;
	FileMgr FManager(Path, DStore);
	FManager.addPattern(Pattern);
	FManager.search();
	for (auto File : DStore)
	{
		this->MA.GranularMetricAnalyzerforFile(File, true, true);  // Analyzing each file that matched with the pattern
	}
}

// Method to get each file for pattern to perform analysis, counting all scopes
void MetricsExecutive::PerformGranularAnalysis(int argc, char* argv[])
{
	std::string Path = argv[1];
	std::vector<std::string> Patterns;
	for (int i = 2; i < argc; ++i)
		Patterns.push_back(argv[i]);
	for (std::string Pattern : Patterns)
	{
		FindAndAnalyzeFiles(Path, Pattern);
	}
}

// Method to send each file matching the pattern in a given directory to be analyzedd for function scopes
void MetricsExecutive::FindAndAnalyzeFilesFull(std::string Path, std::string Pattern)
{
	std::cout << "\n  Metrics Executive - Analyzing " << Pattern << " files at path: " << Path << "\n\n";
	DataStore DStore;
	FileMgr FManager(Path, DStore);
	FManager.addPattern(Pattern);
	FManager.search();
	for(auto File : DStore)
	{
		this->MA.FullMetricAnalyzerforFile(File, true, true);  // Analyzing each file that matched with the pattern
	}
}

// Method to get each file for pattern to perform analysis, counting only function scopes
void MetricsExecutive::PerformFullAnalysis(int argc, char* argv[])
{
	std::string Path = argv[1];
	std::vector<std::string> Patterns;
	for (int i = 2; i < argc; ++i)
		Patterns.push_back(argv[i]);
	for(std::string Pattern : Patterns)
	{
		FindAndAnalyzeFilesFull(Path, Pattern);
	}
}

// Method to display all files in a specified directory matching with a particular pattern or patterns
void MetricsExecutive::Displayfiles(int argc, char* argv[])
{
	std::string Path = argv[1];
	std::vector<std::string> Patterns;
	for (int i = 2; i < argc; ++i)
		Patterns.push_back(argv[i]);
	for(std::string Pattern : Patterns)
	{
		std::cout << "\n  Displaying names of all " << Pattern << " files at path: " << Path << "\n\n";
		DataStore DStore;
		FileMgr FManager(Path, DStore);
		FManager.addPattern(Pattern);
		FManager.search();
		if(DStore.isEmpty())
			std::cout << "  No file was found with this extension.\n" << std::endl;  // Displaying each file that matched with the pattern
		else
		{
			for(auto File : DStore)
			{
				std::cout << "  " << File << std::endl;  // Displaying each file that matches with the pattern
			}
		}
	}
}

// Test stub for Metrics Executive
// Make projects, it depends on, as static libraries in order to execute test stub. Make current project as an application.

#ifdef TEST_EXECUTIVES
int main(int argc, char* argv[])
{
	Scanner::MetricsExecutive ME;
	ME.PerformGranularAnalysis(argc, argv);
	ME.PerformFullAnalysis(argc, argv);
	return 0;
}
#endif 