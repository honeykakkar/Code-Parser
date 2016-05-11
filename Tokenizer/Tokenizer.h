#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              ////////////
// Application:  Lexical Scanner, CSE687 - Object Oriented Design, Project 1  //
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
// Source:      Jim Fawcett, CST 4-187, Syracuse University                  ///////
//              (315) 443-3948, jfawcett@twcny.rr.com                             //
////////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 * It is based on the State Design Pattern. 
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 3.3 : Feb 8 2016
 * - added ActivityTitle and SetAT() method to display grainular activity
 * - Now, it returns quoted strings as single token.
 * - and returns [, ], {, }, (, ), <, >, :, ; as single character tokens
 * - and returns <<, >>, +=, -=, *=, /=, :: as two character tokens

 * ver 3.2 : 28 Jan 2016
 * - fixed bug in ConsumeState::nextState() by returning a valid state
 *   pointer if all tests fail due to reaching end of file instead of
 *   throwing logic_error exception.
 * ver 3.1 : 27 Jan 2016
 * - fixed bug in EatCComment::eatChars()
 * - removed redundant statements assigning _pState in derived eatChars() 
 *   functions
 * - removed calls to nextState() in each derived eatChars() and fixed
 *   call to nextState() in ConsumeState::consumeChars()
 * ver 3.0 : 11 Jun 2014
 * - first release of new design
 */

#include <iosfwd>
#include <string>

namespace Scanner
{
	class ConsumeState; // Forward Declaration
	struct Context; 
	class Toker
	{
	public:
		Toker();
		Toker(const Toker&) = delete;  // To prevent making copies of Toker
		~Toker();
		Toker& operator=(const Toker&) = delete; // To prevent any assignments of Toker
		bool attach(std::istream* pIn);
		std::string getTok();
		bool canRead();
		size_t currentLineCount();
		void setSpecialSingleChars(std::string SSC);
		void setSpecialSinglePairs(std::string SPC);
		bool isComment(const std::string& tok);
		static bool ActivityTitle;  // Flag used to know activity at granul level
		static void SetAT(bool F)   // Method to set the above mentioned flag
		{
			ActivityTitle = F;
		}
	private:
		ConsumeState* pConsumer;
		Context* _pContext;		
	};
}
#endif

