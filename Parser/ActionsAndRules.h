#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.1                                                        //                                                      //
// Language:    C++, Visual Studio 2015                            //
// Application: Lexical Scanner, CSE687 - Object Oriented Design  ////////////
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
// Source:      Jim Fawcett, CST 4-187, Syracuse University                  //
//              (315) 443-3948, jfawcett@twcny.rr.com                        //
///////////////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv Parser.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================

  ver 2.2 : Mar 15,2016
  - added new rules and actions to parse
  - changed linecount to two parameters
  - integrated AST implementation

  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../SemiExp/ITokCollection.h"
#include "../Parser/ASTNode.h"
#include "../ASTree/AbstractSyntaxTree.h"

// ScopeStack element is application specific
// Repository instance is used to share resources among all actions.

namespace Scanner
{
	class Repository  // application specific
	{
		ScopeStack<element*> stack;
		Scanner::Toker* p_Toker;
		AbstractSyntaxTree Tree; 
	public:
		Repository(Scanner::Toker* pToker)
		{
			p_Toker = pToker;
			element* Global = new element;  // Creating global namespace element
			Global->type = "namespace";
			Global->name = "Global";
			Global->startLineCount = 1;
			Global->endLineCount = 0;
			stack.push(Global);  // pushing element on scope stack and making it root for AST
			Tree.SetRoot(Global);
		}

		//Method to get Tree instance of Repository
		AbstractSyntaxTree* GetTree()
		{
			return &Tree;
		}

		//Method to get ScopeStack instance of Repository
		ScopeStack<element*>& scopeStack()
		{
			return stack;
		}

		//Method to get Toker instance of Repository
		Scanner::Toker* Toker()
		{
			return p_Toker;
		}
		size_t lineCount()
		{
			return (size_t)(p_Toker->currentLineCount());
		}
	};

	// rule to detect beginning of anonymous scope
	class BeginningOfScope : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("{") < pTc->length())
			{
				doActions(pTc);
				return true;
			}
			return true;
		}
	};

	// action to handle scope stack at end of scope
	class HandlePush : public IAction
	{
		Repository* p_Repos;
	public:
		HandlePush(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			element* elem = new element;
			elem->type = "Unknown";
			elem->name = "Anonymous";
			elem->startLineCount = p_Repos->lineCount();
			element *Top = p_Repos->scopeStack().top();
			Top->GetChildren().push_back(elem);
			p_Repos->scopeStack().push(elem);
		}
	};

	// rule to detect end of scope
	class EndOfScope : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("}") < pTc->length())
			{
				doActions(pTc);
				return true;
			}
			return true;
		}
	};

	// action to handle scope stack at end of scope
	class HandlePop : public IAction
	{
		Repository* p_Repos;
	public:
		HandlePop(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			if (p_Repos->scopeStack().size() == 0)
				return;
			element* elem = p_Repos->scopeStack().pop();
			elem->endLineCount = p_Repos->lineCount();
		}
	};

	// rule to detect preprocessor statements
	class PreprocStatement : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("#") < pTc->length())
			{
				doActions(pTc);
				return true;
			}
			return true;
		}
	};

	// action to print preprocessor statement to console
	class PrintPreproc : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
		}
	};

	// rule to detect function definitions
	class FunctionDefinition : public IRule
	{
	public:
		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch" };
			for (int i = 0; i < 5; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("(");
				if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
				{
					doActions(pTc);
					return true;
				}
			}
			return true;
		}
	};

	// action to push function name onto ScopeStack
	class PushFunction : public IAction
	{
		Repository* p_Repos;
	public:
		PushFunction(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			p_Repos->scopeStack().pop();
			p_Repos->scopeStack().top()->GetChildren().pop_back();
			std::string name = (*pTc)[pTc->find("(") - 1];

			element* elem = new element;
			elem->type = "function";
			elem->name = name;
			elem->startLineCount = p_Repos->lineCount();
			element *Top = p_Repos->scopeStack().top();
			Top->GetChildren().push_back(elem);
			p_Repos->scopeStack().push(elem);
		}
	};

	// action to send semi-expression that starts a function def to console
	class PrintFunction : public IAction
	{
		Repository* p_Repos;
	public:
		PrintFunction(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::cout << "\n  FuncDef: " << pTc->show().c_str();
		}
	};

	// action to send signature of a function def to console
	class PrettyPrintFunction : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			pTc->remove("public");
			pTc->remove(":");
			pTc->trimFront();
			int len = pTc->find(")");
			std::cout << "\n\n  Pretty Stmt:    ";
			for (int i = 0; i < len + 1; ++i)
				std::cout << (*pTc)[i] << " ";
			std::cout << "\n";
		}
	};
	// Class to detect whether current SemiExp contains class or struct declaration or definition
	class Class_Detection : public IRule
	{
	protected:
		int i=-1;
		std::string keys[2] = { "struct", "class" };
	public:
		// Method to find whether current SemiExp contains class or struct declaration or definition
		bool DeclarationFound(ITokCollection& tc)
		{
			
			for (int j=0; j < 2; ++j)
			{
				size_t Location = tc.find(keys[j]);
				if (Location != tc.length())
				{
					this->i = j;
					return true;
				}
			}
			return false;
		}
		// Method to which type of declaration it is - class or struct
		std::string WhichDeclaration()
		{
			if(this->i !=-1)
				return keys[i];
			return "";
		}

		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				if (DeclarationFound(tc))
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};

	// Class to push scope of class onto ScopeStack and AST
	class PushClass : public IAction
	{
		Repository* p_Repos;
		Class_Detection CD;
	public:
		PushClass(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			// Popping anonymous scope nodes from ScopeStack and AST
			ITokCollection& tc = *pTc;
			p_Repos->scopeStack().pop();
			p_Repos->scopeStack().top()->GetChildren().pop_back();
			element* elem = new element;
			if (CD.DeclarationFound(tc))
			{
				// Create a new element with name, type etc. as detected by parser
				// and eventually pushing it on ScopeStack and AST
				elem->type = CD.WhichDeclaration();
				elem->name = (*pTc)[pTc->find(elem->type) + 1];
				elem->startLineCount = p_Repos->lineCount();
				((p_Repos->scopeStack()).top()->GetChildren()).push_back(elem);
				p_Repos->scopeStack().push(elem);
			}
		}
	};

	// Class to detect scope of namespace in a file
	class Namespace_Detection : public IRule
	{
	public:
		// Method to detect whether namespace keyword is encountered
		bool DeclarationFound(ITokCollection& tc)
		{
			size_t Location = tc.find("namespace");
			if (Location != tc.length())
				return true;
			return false;
		}

		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				if (DeclarationFound(tc))
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};

	// Method to push namespace scope node onto scopestack and AST
	class PushNamespace : public IAction
	{
		Repository* p_Repos;
		Namespace_Detection ND;
	public:
		PushNamespace(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			// Popping anonymous scope nodes from ScopeStack and AST
			ITokCollection& tc = *pTc;
			p_Repos->scopeStack().pop();
			p_Repos->scopeStack().top()->GetChildren().pop_back();
			element* elem = new element;
			if (ND.DeclarationFound(tc))
			{
				// Create a new element with name, type etc. as detected by parser
				// and eventually pushing it on ScopeStack and AST
				elem->type = "namespace";
				elem->name = (*pTc)[pTc->find(elem->type) + 1];
				elem->startLineCount = p_Repos->lineCount();
				((p_Repos->scopeStack()).top()->GetChildren()).push_back(elem);
				p_Repos->scopeStack().push(elem);
			}
		}
	};

	// rule to detect any declaration
	class Declaration : public IRule          // declar ends in semicolon
	{                                         // has type, name, modifiers &
	public:                                   // initializers.  So eliminate
		bool isModifier(const std::string& tok) // modifiers and initializers.
		{                                       // If you have two things left
			const size_t numKeys = 22;            // its declar else executable.
			const static std::string keys[numKeys] = {
			  "const", "extern", "friend", "mutable", "signed", "static",
			  "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			  "public", "protected", "private", ":", "typename", "typedef", "++", "--"
			};
			for (int i = 0; i < numKeys; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}
		void condenseTemplateTypes(ITokCollection& tc)
		{
			size_t start = tc.find("<");
			size_t end = tc.find(">");
			if (start >= end || start == 0)
				return;
			else
			{
				if (end == tc.length())
					end = tc.find(">::");
				if (end == tc.length())
					return;
			}
			std::string save = tc[end];
			std::string tok = tc[start - 1];
			for (size_t i = start; i < end + 1; ++i)
				tok += tc[i];
			for (size_t i = start; i < end + 1; ++i)
				tc.remove(start);
			if (save == ">::")
			{
				tok += tc[start + 1];
				tc.remove(start);
			}
			tc[start - 1] = tok;
		}
		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch" };
			for (int i = 0; i < 5; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}
		void removeInvocationParens(ITokCollection& tc)
		{
			size_t start = tc.find("(");
			size_t end = tc.find(")");
			if (start >= end || end == tc.length() || start == 0)
				return;
			if (isSpecialKeyWord(tc[start - 1]))
				return;
			for (size_t i = start; i < end + 1; ++i)
				tc.remove(start);
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& in = *pTc;
			Scanner::SemiExp tc;
			for (size_t i = 0; i < in.length(); ++i)
				tc.push_back(in[i]);

			if (tc[tc.length() - 1] == ";" && tc.length() > 2)
			{
				removeInvocationParens(tc);
				condenseTemplateTypes(tc);
				Scanner::SemiExp se;
				for (size_t i = 0; i < tc.length(); ++i)
				{
					if (isModifier(tc[i]) || se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
						continue;
					if (tc[i] == "=" || tc[i] == ";")
						break;
					else
						se.push_back(tc[i]);
				}
				if (se.length() == 2)  // type & name, so declaration
				{
					doActions(pTc);
					return true;
				}
			}
			return true;
		}
	};

	// action to show declaration
	class ShowDeclaration : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			// remove comments
			Scanner::SemiExp se;
			for (size_t i = 0; i < tc.length(); ++i)
				if (!se.isComment(tc[i]))
					se.push_back(tc[i]);
			// show cleaned semiExp
			std::cout << "\n  Declaration: " << se.show();
		}
	};

	// rule to detect executable
	class Executable : public IRule           
	{                                         
	public:                                   
		bool isModifier(const std::string& tok)
		{           
			const size_t numKeys = 22;
			const static std::string keys[numKeys] = {
			  "const", "extern", "friend", "mutable", "signed", "static",
			  "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			  "public", "protected", "private", ":", "typename", "typedef", "++", "--"
			};
			for (int i = 0; i < numKeys; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}
		void condenseTemplateTypes(ITokCollection& tc)
		{
			size_t start = tc.find("<");
			size_t end = tc.find(">");
			if (start >= end || start == 0)
				return;
			else
			{
				if (end == tc.length())
					end = tc.find(">::");
				if (end == tc.length())
					return;
			}
			std::string save = tc[end];
			std::string tok = tc[start - 1];
			for (size_t i = start; i < end + 1; ++i)
				tok += tc[i];
			for (size_t i = start; i < end + 1; ++i)
				tc.remove(start);
			if (save == ">::")
			{
				tok += tc[start + 1];
				tc.remove(start);
			}
			tc[start - 1] = tok;
		}

		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch" };
			for (int i = 0; i < 5; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}
		void removeInvocationParens(ITokCollection& tc)
		{
			size_t start = tc.find("(");
			size_t end = tc.find(")");
			if (start >= end || end == tc.length() || start == 0)
				return;
			if (isSpecialKeyWord(tc[start - 1]))
				return;
			for (size_t i = start; i < end + 1; ++i)
				tc.remove(start);
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& in = *pTc;
			Scanner::SemiExp tc;
			for (size_t i = 0; i < in.length(); ++i)
				tc.push_back(in[i]);

			if (tc[tc.length() - 1] == ";" && tc.length() > 2)
			{
				removeInvocationParens(tc);
				condenseTemplateTypes(tc);
				Scanner::SemiExp se;
				for (size_t i = 0; i < tc.length(); ++i)
				{
					if (isModifier(tc[i]) || se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
						continue;
					if (tc[i] == "=" || tc[i] == ";")
						break;
					else
						se.push_back(tc[i]);
				}
				if (se.length() != 2)  // not a declaration
				{
					doActions(pTc);
					return true;
				}
			}
			return true;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to show executable

	class ShowExecutable : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			Scanner::SemiExp se;
			for (size_t i = 0; i < tc.length(); ++i)
			{
				if (!se.isComment(tc[i]))
					se.push_back(tc[i]);
			}
			std::cout << "\n  Executable: " << se.show();
		}
	};


	class Control : public IRule
	{
	public:
		const std::string keys[6] = { "for", "while", "switch", "if", "catch", "else" };
		bool isControlKeyWord(const std::string& tok)
		{
			for (int i = 0; i < 6; ++i)
				if (tok == keys[i])
					return true;
			return false;
		}

		bool isTryBlock(SemiExp& tc)
		{
			size_t tryposition = tc.find("try");
			if (tryposition != tc.length())
			{
				++tryposition;
				while (tc[tryposition] == " " || tc[tryposition] == "\n")
					++tryposition;
				if (tc[tryposition] == "{")
					return true;
			}
			return false;
		}

		int WhichControlStatement(std::string &control)
		{
			if (control == "for")
				return 0;
			if (control == "while")
				return 1;
			if (control == "switch")
				return 2;
			if (control == "if")
				return 3;
			if (control == "catch")
				return 4;
			if (control == "else")
				return 5;
			return -1;
		}

		int isControlStatement(SemiExp& tc)
		{
			size_t parenthesis = tc.find("(");
			size_t i = 0;
			if (parenthesis != tc.length())
			{
				for (; i < parenthesis; ++i)
				{
					if ((tc[i] != " " || tc[i] != "\n") && isControlKeyWord(tc[i]))
						break;
				}
			}
			return WhichControlStatement(tc[i]);
		}

		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& in = *pTc;
			Scanner::SemiExp tc;
			for (size_t i = 0; i < in.length(); ++i)
				tc.push_back(in[i]);

			if ((isControlStatement(tc) != -1 || isTryBlock(tc)) && tc.find("{") != tc.length())
			{
				doActions(pTc);
				return true;
			}
			return false;
		}
	};
	//const static std::string keys[]
	//	= { "for", "while", "switch", "if", "catch" };

	class PushControl : public IAction
	{
		Repository* p_Repos;
	public:
		PushControl(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			p_Repos->scopeStack().pop();
			p_Repos->scopeStack().top()->GetChildren().pop_back();
			std::string name;
			if(pTc->find("(") != pTc->length())
				name = (*pTc)[pTc->find("(") - 1];
			else
			{
				int CL = pTc->find("{") - 1; 
				while ((*pTc)[CL] == " " || (*pTc)[CL] == "\n")
					--CL;
				name = (*pTc)[CL];
			}

			// Create a new element with name, type etc. as detected by parser
			// and eventually pushing it on ScopeStack and AST
			element* elem = new element;
			elem->type = "control scope";
			elem->name = name;
			elem->startLineCount = p_Repos->lineCount();
			element *Top = p_Repos->scopeStack().top();
			Top->GetChildren().push_back(elem);
			p_Repos->scopeStack().push(elem);
		}
	};
}
#endif
