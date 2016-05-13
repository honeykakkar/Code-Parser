/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2016                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

namespace Scanner
{
	//----< destructor releases all parts >------------------------------

	ConfigParseToConsole::~ConfigParseToConsole()
	{
		// when Builder goes out of scope, everything must be deallocated

		delete pHandlePush;
		delete pBeginningOfScope;
		delete pHandlePop;
		delete pEndOfScope;
		delete pPrintFunction;
		delete pPushFunction;
		delete pFunctionDefinition;
		delete pDeclaration;
		delete pShowDeclaration;
		delete pExecutable;
		delete pShowExecutable;
		delete pRepo;
		delete pParser;
		delete pSemi;
		delete pToker;
		delete pClassDetection;
		delete pPushClass;
		delete pControl;
		delete pPushControl;
		delete pNSDetection;
		delete pPushNamespace;
		pIn->close();
		delete pIn;
	}

	//----< attach toker to a file stream or stringstream >------------
	bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
	{
		if (pToker == 0)
			return false;
		pIn = new std::ifstream(name);
		if (!pIn->good())
			return false;
		return pToker->attach(pIn);
	}

	void ConfigParseToConsole::AddRulestoParser()
	{
		pBeginningOfScope = new BeginningOfScope();
		pHandlePush = new HandlePush(pRepo);
		pBeginningOfScope->addAction(pHandlePush);
		pParser->addRule(pBeginningOfScope);
		pEndOfScope = new EndOfScope();
		pHandlePop = new HandlePop(pRepo);
		pEndOfScope->addAction(pHandlePop);
		pParser->addRule(pEndOfScope);

		pFunctionDefinition = new FunctionDefinition;
		pPushFunction = new PushFunction(pRepo);  // no action
		pPrintFunction = new PrintFunction(pRepo);
		pFunctionDefinition->addAction(pPushFunction);
		pFunctionDefinition->addAction(pPrintFunction);
		pParser->addRule(pFunctionDefinition);

		pDeclaration = new Declaration;
		pShowDeclaration = new ShowDeclaration;
		pDeclaration->addAction(pShowDeclaration);
		pParser->addRule(pDeclaration);
		pExecutable = new Executable;
		pShowExecutable = new ShowExecutable;
		pExecutable->addAction(pShowExecutable);
		pParser->addRule(pExecutable);

		pClassDetection = new Class_Detection;
		pPushClass = new PushClass(pRepo);
		pClassDetection->addAction(pPushClass);
		pParser->addRule(pClassDetection);

		pNSDetection = new Namespace_Detection;
		pPushNamespace = new PushNamespace(pRepo);
		pNSDetection->addAction(pPushNamespace);
		pParser->addRule(pNSDetection);

		pControl = new Control;
		pPushControl = new PushControl(pRepo);
		pControl->addAction(pPushControl);
		pParser->addRule(pControl);
	}

	//----< Here's where all the parts get assembled >----------------
	Parser* ConfigParseToConsole::Build()
	{
		try
		{
			// add Parser's main parts
			pToker = new Toker;
			//pToker->returnComments(false);
			pSemi = new SemiExp(pToker);
			pParser = new Parser(pSemi);
			pRepo = new Repository(pToker);
			AddRulestoParser();
			// configure to manage scope these must come first - they return true on match so rule checking continues
			return pParser;
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n  " << ex.what() << "\n\n";
			return 0;
		}
	}

	void ConfigParseToConsole::GranularAnalyze()
	{
		pRepo->GetTree()->PerformGranularAnalysis(pRepo->GetTree()->GetRoot());
	}

	void ConfigParseToConsole::FullAnalyze()
	{
		pRepo->GetTree()->PerformFullAnalysis(pRepo->GetTree()->GetRoot());
	}

	void ConfigParseToConsole::DisplayGranularAST()
	{
		std::cout << "\n  Abstract Syntax Tree for this file in format (type, name, startline, scope-size)\n";
		pRepo->GetTree()->TreeWalk(pRepo->GetTree()->GetRoot());
		std::cout << "\n\n";
	}

	void ConfigParseToConsole::DisplayFullAST()
	{
		std::cout << "\n  Abstract Syntax Tree for this file in format (type, name, startline, scope-size)\n";
		pRepo->GetTree()->TreeWalkForFunctions(pRepo->GetTree()->GetRoot());
		std::cout << "\n\n";
	}
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

using namespace Scanner;
int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}
#endif
