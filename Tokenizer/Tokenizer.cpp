/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.3                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Lexical Scanner, CSE687 - Object Oriented Design  ////////////
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
// Source:      Jim Fawcett, CST 4-187, Syracuse University                  //
//              (315) 443-3948, jfawcett@twcny.rr.com                        //
///////////////////////////////////////////////////////////////////////////////
/*
*Tokenizer.cpp: Contains the code to extract tokens as per defined rules from the stream
* Quoted strings and certain punctuators and newlines are returned as single tokens.
*
* Build Process :
*--------------
* Required Files : Tokenizer.h, ConsumeState.h
* Build Command : devenv Tokenizer.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 3.3 : Feb 8 2016
*/

#include "Tokenizer.h"
#include "ConsumeState.h"
#include <iostream>
#include <cctype>
#include <string>
#include<vector>

using namespace Scanner;
bool Toker::ActivityTitle = false;  // Description in its header file. By default, set true

//std::string ConsumeState::token;
//std::istream* ConsumeState::_pIn = nullptr;  // Contains input stream

void testLog(const std::string& msg);


void ConsumeState::setContext(Context* pContext)
{
	_pContext = pContext;
}

size_t Toker::currentLineCount() { return _pContext->_lineCount; }

//Method used to check if character passed as an argument exists in the vector of SingleChars to be recognized as tokens
bool ConsumeState::IfCharExists(char R)
{
	if ((int)R == 39)
		return false;
	std::string T; T.push_back(R);
	for (auto I = _pContext->SpecialSingleChars.begin(); I != _pContext->SpecialSingleChars.end(); ++I)
	{
		if (*I == T)
			return true;
	}
	return false;
}

//Method used to check if string (2 chars) passed as an argument exists in the vector of SpecialCharPairs to be recognized as tokens
bool ConsumeState::IfPairExists(std::string T)
{
	for (auto I = _pContext->SpecialCharPairs.begin(); I != _pContext->SpecialCharPairs.end(); ++I)
	{
		if (*I == T)
			return true;
	}
	return false;
}

bool ConsumeState::TokenCollector()
{
	if (_pContext->_pIn->good())
	{
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\n')
			++(_pContext->_lineCount);
		return true;
	}
	return false;
}

size_t ConsumeState::currentLineCount() { return _pContext->_lineCount; }

void ConsumeState::SetDoReturnComments(bool F)
{
	_pContext->DoReturnComments = F;
}

//Constructor of Token class
Toker::Toker()
{
	_pContext = new Context();
	pConsumer = _pContext->_pEatWhitespace;
	pConsumer->setContext(_pContext);
}

Toker::~Toker()
{
	delete pConsumer;
	_pContext = nullptr;
}


// Class to recognize whitespaces as tokens from the input stream. Usually, they are discarded.
class EatWhitespace : public ConsumeState
{
public:
	EatWhitespace(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		do 
		{
			if (!TokenCollector())  // end of stream
				return;
		} while (std::isspace(_pContext->currChar) && _pContext->currChar != '\n');  // Keeps triming the white spaces till the end
	}
};

// Class to recognize C++-style comments as tokens from the input stream. 
// Can be discarded or saved as token depending upon flag DoReturnComments
class EatCppComment : public ConsumeState
{
public:
	EatCppComment(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars() // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if(Toker::ActivityTitle  == true)
			std::cout << "\n  Tokenization of C++ comment";
		do {
			if (!_pContext->_pIn->good())  // end of stream
				return;
			// Can be discarded or saved as token depending upon flag DoReturnComments
			if (_pContext->DoReturnComments == true)
				_pContext->token += _pContext->currChar;
			if (!TokenCollector())
				return;
		} while (_pContext->currChar != '\n');
	}
};

// Class to recognize C-style comments as tokens from the input stream. 
// Can be discarded or saved as token depending upon flag DoReturnComments
class EatCComment : public ConsumeState
{
public:
	EatCComment(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars() // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if (Toker::ActivityTitle == true)
			std::cout << "\n  Tokenization of C comment";
		do {
			if (!_pContext->_pIn->good())  // end of stream
				return;
			// Can be discarded or saved as token depending upon flag DoReturnComments
			if (_pContext->DoReturnComments == true)
				_pContext->token += _pContext->currChar;
			if (!TokenCollector())
				return;
		} while (!(_pContext->currChar == '*' && _pContext->_pIn->peek() == '/'));

		// Move pointer in the input stream 2 steps ahead to pass ahead the comments
		if (_pContext->DoReturnComments == true)
			_pContext->token += _pContext->currChar;
		if (!TokenCollector())
			return;
		if (_pContext->DoReturnComments == true)
			_pContext->token += _pContext->currChar;
		TokenCollector();
	}
};

// Class to recognize alphanumeric keywords as tokens from the input stream
class EatAlphanum : public ConsumeState
{
public:
	EatAlphanum(Context* pContext)
	{
		_pContext = pContext;
	}

	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if (Toker::ActivityTitle == true)
			std::cout << "\n  Tokenization of AlphaNumeric";
		do {
			_pContext->token += _pContext->currChar;
			if (!_pContext->_pIn->good())  // end of stream
				return;
			if (!TokenCollector())
				return;
		} while (isalnum(_pContext->currChar) || _pContext->currChar == '_');
	}
};

// Class to recognize newlines ("\n") as tokens from the input stream
class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if (Toker::ActivityTitle == true)
			std::cout << "\n  Encounter of newline";
		_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\n')
			++(_pContext->_lineCount);
	}
};

// Class to recognize single characters as tokens from the input stream
class EatSpecialSingleChar : public ConsumeState
{
public:
	EatSpecialSingleChar(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if (Toker::ActivityTitle == true)
			std::cout << "\n  Tokenization of Single Special Character";
		_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())  // end of stream
			return;
		if (!TokenCollector())
			return;		
	}
};

// Class to recognize Character pairs ( 2 chars) as tokens from the input stream
class EatSpecialCharPair : public ConsumeState
{
public:
	EatSpecialCharPair(Context* pContext)
{
	_pContext = pContext;
}
	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if (Toker::ActivityTitle == true)
			std::cout << "\n  Tokenization of pair of Special Characters";
		_pContext->token += _pContext->currChar;
		_pContext->token += _pContext->_pIn->get();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		if (!TokenCollector())
			return;
	}
};

// Class to recognize strings (double quotes) as tokens from the input stream
class EatDoubleQuote : public ConsumeState
{
public:
	EatDoubleQuote(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		if (Toker::ActivityTitle == true)
			std::cout << "\n  Tokenization of double quotes (strings)";
		do
		{
			_pContext->token += _pContext->currChar;
			if (!TokenCollector())
				return;
		} while (_pContext->currChar != '\"');
		_pContext->token += _pContext->currChar;
		TokenCollector();
	}
};

// Class to recognize punctuators as tokens from the input stream
class EatPunctuator : public ConsumeState
{
public:
	EatPunctuator(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()   // Overridden function of this state inherited from Base class ConsumeState
	{
		_pContext->token.clear();
		do 
		{
			if (_pContext->currChar == '\"' && _pContext->prevChar != '\\')    // start of double quoted string
			{
				return;
			}
			if (_pContext->currChar == '\'' && _pContext->prevChar != '\\')    // start of single quoted string
			{
				return;
			}
			if (Toker::ActivityTitle == true)
				std::cout << "\n  Tokenization of punctuator";
			_pContext->token += _pContext->currChar;
			if (!_pContext->_pIn->good())  // end of stream
			{
				return;
			}
			if (!TokenCollector())
				return;
		} while (ispunct(_pContext->currChar));
	}
};

// Method to get the next state in which tokenizer should enter based on its current character
ConsumeState* ConsumeState::nextState()
{
	if (_pContext->currChar == -1)
		_pContext->currChar = _pContext->_pIn->peek();
	if (!(_pContext->_pIn->good()))
	{
		return nullptr;
	}
	int chNext = _pContext->_pIn->peek();
	if (chNext == EOF)
	{
		_pContext->_pIn->clear();
		// if peek() reads end of file character, EOF, then eofbit is set and
		// _pIn->good() will return false.  clear() restores state to good
	}

	// If current char is a whitespace, enter the WhiteSpace state
	if (std::isspace(_pContext->currChar) && _pContext->currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pContext->_pEatWhitespace;
	}

	// Check whether it is a beginning of CPP-style comment, Enter EatCPPComment state if yes.
	if (_pContext->currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pContext->_pEatCppComment;
	}

	// Check whether it is a beginning of C-style comment, Enter EatCComment state if yes.
	if (_pContext->currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pContext->_pEatCComment;
	}

	//If current char is a new line, enter Newline state
	//if (currChar == '\n')
	//{
	//	testLog("state: eatNewLine");
	//	return _pEatNewline;
	//}
	// It has been commented as newline is part of SingleChar now. Upon encounter of "\n", next state enters into EatSingleChar state

	// If it is alphanumeric keyword, enter Alphanum state
	if (std::isalnum(_pContext->currChar))
	{
		testLog("state: eatAlphanum");
		return _pContext->_pEatAlphanum;
	}

	// If current char is a double quote, enter string or double quote state
	if (_pContext->currChar == '\"')
	{
		testLog("state: eatDoubleQuote");
		return _pContext->_pEatDoubleQuote;
	}

	// Enter CharPair state if and only if current character and next character make a pair already existing in vector of CharPairs
	std::string T;
	T.push_back(_pContext->currChar); T.push_back(chNext);
	if ((chNext == '=' || chNext == _pContext->currChar) && IfPairExists(T))
	{
		testLog("state: eatSpecialCharsPairs");
		return _pContext->_pEatSpecialCharPair;
	}

	// Enter single char state only if after appending the next character, it doesn't make it a double char token
	// For this reason, this method checks for CharPairs first
	if (IfCharExists(char(_pContext->currChar)) && (chNext != '=' || chNext != _pContext->currChar))
	{
		testLog("state: eatSpecialSingleChars");
		return _pContext->_pEatSpecialSingleChar;
	}

	// Enter punctuator state only if it is a punctuator and doesn't belong to SingleChar tokens and 2-char tokens
	if (_pContext->currChar != '\"' && ispunct(_pContext->currChar) && !IfCharExists(char(_pContext->currChar)))
	{
		testLog("state: eatPunctuator");
		return _pContext->_pEatPunctuator;
	}
	
	if (!_pContext->_pIn->good())
	{
		//std::cout << "\n  end of stream with currChar = " << currChar << "\n\n";
		return _pContext->_pEatWhitespace;
	}

	std::cout << "\n  Couldn't find any ConsumerState to eat characters";
	return nullptr;
}

Context::Context()
{
	_pEatAlphanum = new EatAlphanum(this);
	_pEatCComment = new EatCComment(this);
	_pEatCppComment = new EatCppComment(this);
	_pEatPunctuator = new EatPunctuator(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pEatSpecialSingleChar = new EatSpecialSingleChar(this);
	_pEatSpecialCharPair = new EatSpecialCharPair(this);
	_pState = _pEatWhitespace;
	_pEatDoubleQuote = new EatDoubleQuote(this);
	_lineCount = 1;
	DoReturnComments = false;
}
//----< return shared resources >------------------------------------

Context::~Context()
{
	delete _pEatAlphanum;
	delete _pEatCComment;
	delete _pEatCppComment;
	delete _pEatPunctuator;
	delete _pEatWhitespace;
	delete _pEatSpecialSingleChar;
	delete _pEatSpecialCharPair;
	delete _pEatDoubleQuote;
}

// Attaches the input stream
bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

// Retrieves tokens based on the state
std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

bool Toker::isComment(const std::string& tok)
{
	if (tok.find("//") < tok.size() || tok.find("/*") < tok.size())
		return true;
	return false;
}

// Method to add 1 char strings to be recognized as a token by Tokenizer
bool Scanner::ConsumeState::setSpecialSingleChars(std::string SSC)
{
	_pContext->SpecialSingleChars.push_back(SSC);
	return true;
}

// Method to add pairs of 2 chars to be recognized as a token by Tokenizer
bool Scanner::ConsumeState::setSpecialSinglePairs(std::string SPC)
{
	_pContext->SpecialCharPairs.push_back(SPC);
	return true;
}

void Toker::setSpecialSingleChars(std::string SSC)
{
	pConsumer->setSpecialSingleChars(SSC);
}

void Toker::setSpecialSinglePairs(std::string SSP)
{
	pConsumer->setSpecialSinglePairs(SSP);
}


void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZERS

#include <fstream>

int main()
{
  std::string fileSpec = "../TestFiles/AbstractSyntaxTree.cpp";
  std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.attach(&in);
  while (in.good())
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  }
  std::cout << "\n\n";
  return 0;
}
#endif
