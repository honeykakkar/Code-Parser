///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Honey Kakkar, Computer Engineering, SU               //
//              hkakkar@syr.edu                                      //
// Source:      Jim Fawcett, CST 4-187, Syracuse University          //
//              (315) 443-3948, jfawcett@twcny.rr.com                //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;
//using Token = std::string;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {} // Initializing _pToker using initializer list
Token SemiExp::operator[](size_t n)
{
  if (n < 0 || n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

bool SemiExp::isSemiColonBetweenParens()
{
	size_t openParenPosition = find("(");
	size_t semiColonPosition = find(";");
	size_t closedParenPosition = find(")");

	if (openParenPosition < semiColonPosition && semiColonPosition < closedParenPosition)
		return true;
	return false;
}

bool SemiExp::get(bool clear)
{
	bool ok = getHelper(clear);

	if (hasFor && isSemiColonBetweenParens())
	{
		getHelper(false);  // add test for loop termination
		getHelper(false);  // add counter increment
	}
	return ok;
}
//----< is token a SemiExpression terminator? >----------------------

bool SemiExp::isTerminator(const std::string& token)
{
	if (token == "{" || token == "}" || token == ";")
		return true;

	if (token == "\n")
	{
		trimFront();
		if (!_tokens.empty() && _tokens[0] == "#")
			return true;
	}

	if (length() < 2)
		return false;

	if (token == ":" && length() > 0 && _tokens[length() - 2] == "public")
		return true;

	if (token == ":" && length() > 0 && _tokens[length() - 2] == "protected")
		return true;

	if (token == ":" && length() > 0 && _tokens[length() - 2] == "private")
		return true;

	return false;
}
//----< does all the work of collecting tokens for collection >------

bool SemiExp::getHelper(bool clear)
{
	hasFor = false;
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	if (clear)
		_tokens.clear();
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token == "")
			break;
		_tokens.push_back(token);

		if (token == "for")
			hasFor = true;

		if (isTerminator(token))
			return true;
	}
	return false;
}

// ITokCollection Methods
bool SemiExp::SearchCollection(std::string T)
{
	for (auto I = _tokens.begin(); I != _tokens.end(); ++I)
	{
		if (*I == T)
			return true;
	}
	return false;
}

size_t SemiExp::length()  // It returns the number of tokens in the vector which represents a semi-expression
{
	return _tokens.size();
}

bool SemiExp::isComment(const std::string& tok)
{
	return _pToker->isComment(tok);
}

std::string & Scanner::SemiExp::operator[](int n)
{
	size_t N = (size_t)n;
	if (n < 0 || n >= ((int)_tokens.size()))
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

size_t SemiExp::find(const std::string & tok)  // It checks for presence of string passed in the vector of tokens
{
	auto I = _tokens.begin();
	for (; I != _tokens.end(); ++I)
	{
		if (*I == tok)
			break;
	}
	if (I == _tokens.end())
		return length();
	return I - _tokens.begin();
}

void SemiExp::push_back(const std::string & tok)
{
	this->_tokens.push_back(tok);
}

bool SemiExp::merge(const std::string & firstTok, const std::string & secondTok)
{
	return false;
}

bool SemiExp::remove(const std::string & tok)
{
	size_t i = find(tok);
	if (!(i >= 0 && i < _tokens.size()))
		return false;
	_tokens.erase(i + _tokens.begin());
	return true;		
}

bool SemiExp::remove(size_t i)
{
	if (!(i >= 0 && i < _tokens.size()))
		return false;
	_tokens.erase(i + _tokens.begin());
	return true;
}

void SemiExp::toLower()
{
	for (auto I = _tokens.begin(); I != _tokens.end(); ++I)
	{
		for (size_t i = 0; i < I->length(); ++i)
		{
			if (I->at(i) >= 65 && I->at(i) <= 90)
				I->at(i) += 32;
		}
	}
}

void SemiExp::trimFront()
{
	for (auto I = _tokens.begin(); I != _tokens.end();)
	{
		if (*I != "\n")
			break;
		else
			I = _tokens.erase(I);
	}
}

void SemiExp::clear()
{
	_tokens.clear();
}

std::string SemiExp::show(bool showNewLines)
{
	std::string Result = "\n  ";
	for (auto token : _tokens)
	{
		if (showNewLines == false && token != "\n")
			Result += token;
		if (showNewLines == true)
			Result += token;
		Result += " ";
	}
	Result += "\n";
	return Result;
}

// End of implementation of ITokCollection interface

// Test Stub for SemiExp
#ifdef TEST_SEMIEXPS
int main()
{
  Toker toker;
  std::string fileSpec = "../TestFiles/AbstractSyntaxTree.cpp";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);

  SemiExp semi(&toker);
  while(semi.get())
  {
    std::cout << "\n  -- semiExpression --";
	semi.show(false);
  }
  /*
     May have collected tokens, but reached end of stream
     before finding SemiExp terminator.
   */
  if (semi.length() > 0)
  {
    std::cout << "\n  -- semiExpression --";
    semi.show(false);
    std::cout << "\n\n";
  }
  return 0;
}
#endif