#ifndef CONSUMESTATE_H
#define CONSUMESTATE_H
//////////////////////////////////////////////////////////////////////////
// ConsumeState.h - read words from a std::stream                       //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2015                                 /////////
// Application:  Lexical Scanner, CSE687 - Object Oriented Design, Project 1  //
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public ConsumeState class.
* It manages the state variables according to the current character in input stream
* State variables are static. It also contains vectors for single characters and double characters.
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Tokenizer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : Feb 8 2016
*/

#include <iosfwd>
#include <string>
#include <vector>
namespace Scanner
{

	///////////////////////////////////////////////////////////////////
	// Context is a shared data storage facility.
	/*
	* - with Context we don't need static members to share
	*   state pointes etc.  Context holds and shares all that.
	* - Context is an entirely private facility used by ConsumeState
	*   and its derived instances, so there is no need for encapsulation
	*   and we use a struct.  This won't be appropriate if we handed
	*   the context to clients of Toker.
	*/
	struct Context
	{
		Context();
		~Context();
		std::string token;
		std::istream* _pIn;
		ConsumeState* _pState;
		std::vector<std::string> SpecialCharPairs = { "<<", ">>" , "::" , "++", "--" , "==", "+=", "-=", "*=", "/=" }; //vector holding 2 char tokens
		std::vector<std::string> SpecialSingleChars = { "<",">","[","]","(",")","{","}",":","=","+","-","*","\n" }; // vector holding 1 char tokens
		int prevChar;
		int currChar;
		size_t _lineCount;
		bool DoReturnComments;   // Flag is checked if user wants C or CPP comments to be considered as tokens
		ConsumeState* _pEatCppComment = nullptr;				//
		ConsumeState* _pEatCComment = nullptr;				//
		ConsumeState* _pEatWhitespace = nullptr;				//
		ConsumeState* _pEatPunctuator = nullptr;				// Different states of ConsumeState to recognize tokens
		ConsumeState* _pEatAlphanum = nullptr;				//
		ConsumeState* _pEatNewline;							//
		ConsumeState* _pEatSpecialSingleChar = nullptr;		//
		ConsumeState* _pEatSpecialCharPair = nullptr;			//
		ConsumeState* _pEatDoubleQuote = nullptr;
		ConsumeState* _pEatSingeQuote = nullptr;
	};

	class ConsumeState
	{
		friend class Toker;
	public:
		using Token = std::string;
		ConsumeState(){}
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState(){}
		void attach(std::istream* pIn) { _pContext->_pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			if (_pContext->_pState != NULL)
				_pContext->_pState->eatChars();
			_pContext->_pState = nextState();
		}
		bool canRead() { return _pContext->_pIn->good(); }
		std::string getTok() { return _pContext->token; }
		bool IfCharExists(char R);
		bool IfPairExists(std::string T);
		bool TokenCollector();
		bool hasTok() { return _pContext->token.size() > 0; }
		ConsumeState* nextState();
		bool setSpecialSingleChars(std::string SSC);
		bool setSpecialSinglePairs(std::string SPC);
		void SetDoReturnComments(bool F);  // Method to set DoReturnComments flag to consider comments as tokens or not
		size_t currentLineCount();
		void setContext(Context* pContext);
	protected:
		Context* _pContext;
	};
}
#endif
