/////////////////////////////////////////////////////////////////////
// AbstractSyntaxTree.h - package to create abstract syntax tree   //                                                        
// Language:    C++, Visual Studio 2015                            //
// Application: Code Parser with AST, CSE687 - Object Oriented Design  ////////
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
*AbstractSyntaxTree.h: Contains the code to declare methods and members required to 
* implement abstract syntax tree for parser
*
* Build Process :
*--------------
* Required Files : ASTNode.h
* Build Command : devenv ASTree.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 16 2016
*/
#ifndef AST_H
#define AST_H

#include<string>
#include<vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../Parser/ASTNode.h"


namespace Scanner
{
	// element object is being used as node for AST
	using ASTNode = element;
	class AbstractSyntaxTree
	{
	private:
		ASTNode *Root;
	public:
		AbstractSyntaxTree()
		{
			Root = NULL;
		}
		// Getters and setters of root
		void SetRoot(ASTNode* root);
		ASTNode* GetRoot();
		void InsertAt(ASTNode *P, ASTNode *C);   //  // To insert a particular node at AST at particular location (not used in current demonstration)
		ASTNode* FindNode(ASTNode *FindIt);  // To find a particular node at AST (not used in current demonstration)
		size_t GetComplexity(ASTNode* node);
		void PerformGranularAnalysis(ASTNode* element);		//  Methods to analyze all scopes and nodes of AST
		void GranularAnalysisHelper(ASTNode* element);		//
		
		void PerformFullAnalysis(ASTNode* element);			// Methods to analyze the scopes and nodes of AST only of function type
		void FullAnalysisHelper(ASTNode* element);			//

		virtual ~AbstractSyntaxTree()
		{
			delete Root;
			Root = nullptr;
		}
		// Traversal of tree displaying all scopes
		void TreeWalk(ASTNode* Root);
		// Traversal of tree displaying only function scopes
		void TreeWalkForFunctions(ASTNode* Root);
	};
}
#endif