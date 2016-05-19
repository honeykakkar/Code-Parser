/////////////////////////////////////////////////////////////////////        //
// MetricsAnalyzer.h - package for evaluating and displaying the size        //
//                     and complexity of all global functions, static        //
//                     member functions, and non-static member functions     //
//                     in each of a set of specified packages                //
// Language:    C++, Visual Studio 2015                                      //
// Application: Code Parser with AST, CSE687 - Object Oriented Design        //
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
*MetricsAnalyzer.h: Contains the code to package for evaluating and displaying 
*  the size and complexity of all global functions, static member functions, and 
*   non-static member functions in each of a set of specified packages
*
* Build Process :
*--------------
* Build Command : devenv MetricsAnalyzer.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 16 2016
*/

#pragma once
#include <vector>
#include <string>
namespace Scanner
{
	class MetricsAnalyzer
	{
	public:
		void FullMetricAnalyzerforFile(std::string File, bool DisplayTree, bool DisplayTable);	// Method to analyze file scopes which are of function type
		void GranularMetricAnalyzerforFile(std::string File, bool DisplayTree, bool DisplayTable);	// Method to analyze fine at very granular level and counting all scopes
		
	};
}
