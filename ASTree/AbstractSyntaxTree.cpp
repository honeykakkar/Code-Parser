////////////////////////////////////////////////////////////////////////
// AbstractSyntaxTree.cpp - package to create abstract syntax tree   //                                                        
// Language:    C++, Visual Studio 2015                            ///////
// Application: Code Parser with AST, CSE687 - Object Oriented Design  ////////
// Author:      Honey Kakkar, Computer Engineering, SU                       //
//              hkakkar@syr.edu                                              //
///////////////////////////////////////////////////////////////////////////////
/*
*AbstractSyntaxTree.cpp: Contains the code to implement and define methods required to
* implement abstract syntax tree for parser
*
* Build Process :
*--------------
* Required Files : ASTNode.h, AbstractSyntaxTree.h
* Build Command : devenv ASTree.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : Mar 16 2016
*/

#include "AbstractSyntaxTree.h"
#include <stack>
#include <set>
using namespace Scanner;

// Method to underline
void Underline(int L)
{
	for (int i = 0; i<L; i++)
		std::cout << "--";
	std::cout << "\n";
}

//Method to retrieve root of the tree
ASTNode* AbstractSyntaxTree::GetRoot()
{
	return this->Root;
}

// Method to set root to the tree
void AbstractSyntaxTree::SetRoot(ASTNode* root)
{
	this->Root = root;
}

//Method to insert a node after a particular node
void AbstractSyntaxTree::InsertAt(ASTNode* P, ASTNode *C)
{
	ASTNode* Parent = FindNode(P);
	if (Parent == nullptr)
		this->Root = C;
	else
		Parent->AddChild(C);
}

// Method to find a particular node in tree. Returns its parent
ASTNode* AbstractSyntaxTree::FindNode(ASTNode* FindIt)
{
	if (this->Root == NULL)
		return NULL;

	// Maintaining a stack to traverse the Tree in DFS manner (non-recursive)
	std::stack<ASTNode*> DFSStack;

	// Maintaining a set of nodes which have been visited during traversal
	std::set<ASTNode*> Visited;

	DFSStack.push(this->Root);
	Visited.insert(this->Root);
	std::vector<ASTNode*> Children;
	while (!DFSStack.empty())
	{
		ASTNode* Top = DFSStack.top();
		DFSStack.pop();
		if ((*FindIt).IfEqual(*Top))
			return Top;
		Children = Top->GetChildren();
		for (ASTNode* child : Children)
		{
			if (Visited.find(child) == Visited.end())
			{
				DFSStack.push(child);
				Visited.insert(child);
			}
		}
	}
	return NULL;
}

// Traversing and displaying each node in tree
void AbstractSyntaxTree::TreeWalk(ASTNode* pItem)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(4 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())
	{
		TreeWalk(*iter);
		++iter;
	}
	--indentLevel;
}

// Traversing and displaying each node with function scopes in tree
void AbstractSyntaxTree::TreeWalkForFunctions(ASTNode* pItem)
{
	static size_t indentLevel = 0;
	if(pItem->type == "function" || pItem->type == "namespace")
		std::cout << "\n  " << std::string(4 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())
	{
		TreeWalkForFunctions(*iter);
		++iter;
	}
	--indentLevel;
}

// Calculating the cyclomatic complexity of ASTree
size_t AbstractSyntaxTree::GetComplexity(ASTNode* node)
{
	int CC = 1;
	if (node->GetChildren().size() == 0)
		return CC;
	for (auto i = node->GetChildren().cbegin(); i != node->GetChildren().cend(); ++i)
		CC += GetComplexity(*i);
	return CC;
}

//Helps in analyzing (in recurssion) of AST
void AbstractSyntaxTree::FullAnalysisHelper(ASTNode* Element)
{
	if (Element->type == "function" || Element->type == "namespace")
	{
		std::cout << std::left << std::setw(20) << Element->type << std::left << std::setw(30) << Element->name
			<< std::left << std::setw(15) << Element->startLineCount << std::left << std::setw(15) << Element->endLineCount
			<< std::left << std::setw(15) << Element->endLineCount - Element->startLineCount + 1
			<< std::left << GetComplexity(Element) << "\n";
	}
		auto iter = Element->_children.begin();
		while (iter != Element->_children.end())
		{
			FullAnalysisHelper(*iter);
			++iter;
		}
}

//Analyzes the Abstract Syntax Tree
void AbstractSyntaxTree::PerformFullAnalysis(ASTNode* Element)
{
	std::cout << std::setw(25) << "";
	std::cout << std::setw(55) <<"Function Metrics Analysis\n";
	std::cout << "COMPLEXITY TABLE\n";
	Underline(55);
	std::cout << std::left << std::setw(20) << "Scope's Type" << std::left << std::setw(30) << "Function's Name"
		<< std::left << std::setw(15) << "StartLine" << std::left << std::setw(15) << "EndLine"
		<< std::left << std::setw(15) << "Size"	<< std::left << "Complexity\n";
	Underline(55);
	FullAnalysisHelper(Element);
	std::cout << std::setw(30) << "\n" << "END OF TABLE\n";
}

//Helps in analyzing (in recurssion) of AST
void AbstractSyntaxTree::GranularAnalysisHelper(ASTNode* Element)
{
	std::cout << std::left << std::setw(20) << Element->type << std::left << std::setw(25) << Element->name << std::left << std::setw(15)
		<< Element->endLineCount - Element->startLineCount + 1
		<< std::left << GetComplexity(Element) << "\n";
	auto iter = Element->_children.begin();
	while (iter != Element->_children.end())
	{
		GranularAnalysisHelper(*iter);
		++iter;
	}
}

//Analyzes the Abstract Syntax Tree
void AbstractSyntaxTree::PerformGranularAnalysis(ASTNode* Element)
{
	std::cout << std::setw(45) << "Granular Metrics Analysis\n";
	std::cout << "  COMPLEXITY TABLE\n";
	Underline(40);
	std::cout << std::left << std::setw(20) << "Scope's Type" << std::left << std::setw(25) << "Scope's Name" << std::left << std::setw(15) << "Size"
		<< std::left << "Complexity\n";
	Underline(40);
	GranularAnalysisHelper(Element);
	std::cout << std::setw(25) << "\n" << "END OF TABLE\n";
}

// Test Stub for AST
#ifdef TEST_ASTS
int main()
{
	AbstractSyntaxTree AST;

	ASTNode Root;
	Root.name = "Root";
	Root.type = "Node";
	Root.startLineCount = 1;
	Root.endLineCount = 0;
	AST.SetRoot(&Root);
	
	ASTNode N1;
	N1.name = "Node 1";
	N1.type = "Node";
	N1.startLineCount = 1;
	N1.endLineCount = 0;
	Root.GetChildren().push_back(&N1);
	
	ASTNode N2;
	N2.name = "Node 2";
	N2.type = "Node";
	N2.startLineCount = 1;
	N2.endLineCount = 0;
	N1.GetChildren().push_back(&N2);


	ASTNode N3;
	N3.name = "Node 3";
	N3.type = "Node";
	N3.startLineCount = 1;
	N3.endLineCount = 0;
	Root.GetChildren().push_back(&N3);	
	AST.TreeWalk(AST.GetRoot());
}

#endif
