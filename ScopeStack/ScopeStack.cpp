///////////////////////////////////////////////////////////////////////////////
// ScopeStack.cpp - implements template stack holding specified element type //
// ver 2.0                                                                   //
// Language:      Visual C++ 2010, SP1                                       //
// Platform:      Dell Precision T7400, Win 7 Pro SP1                        //
// Application:   Code Analysis Research                                     //
// Author:        Jim Fawcett, CST 4-187, Syracuse University                //
//                (315) 443-3948, jfawcett@twcny.rr.com                      //
///////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#include "ScopeStack.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "../ASTree/AbstractSyntaxTree.h"

// Implemented AST

// Test stub for scopestack
// Modified console display statements
using namespace Scanner;

void Display()
{
	std::cout << "  Testing ScopeStock";
}

#ifdef TEST_SCOPESTACKS

int main()
{
  std::cout << "\n  Testing Scope Stack" << "\n =====================\n" << "\n  pushing items onto ScopeStack instance after adding startLineCount" << "\n --------------------------------------------------------------------";
  ScopeStack<element*> testStack;
  element* pItem = new element;
  pItem->type = "function";
  pItem->name = "fun1";
  pItem->startLineCount = 33;
  testStack.push(pItem);

  pItem = new element;
  pItem->type = "if";
  pItem->name = "";
  pItem->startLineCount = 38;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);

  pItem = new element;
  pItem->type = "for";
  pItem->name = "";
  pItem->startLineCount = 40;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);

  showStack<element>(testStack, false);
  std::cout << "\n\n  Popping two items off ScopeStack after adding endLineCount" << "\n ------------------------------------------------------------";
  testStack.top()->endLineCount = 50;
  std::cout << "\n  " << testStack.pop()->show();
  testStack.top()->endLineCount = 55;
  std::cout << "\n  " << testStack.pop()->show() << "\n";
  std::cout << "\n  Pushing another item onto ScopeStack after adding startLineElement" << "\n --------------------------------------------------------------------";
  pItem = new element;
  pItem->type = "while";
  pItem->name = "";
  pItem->startLineCount = 60;
  (testStack.top()->_children).push_back(pItem);
  testStack.push(pItem);
  std::cout << "\n  Stack now contains:" << "\n ---------------------";
  showStack<element>(testStack, false);
  std::cout << "\n\n  Popping last elements off stack after adding endLineCount" << "\n -----------------------------------------------------------";
  testStack.top()->endLineCount = 70;
  element* pTop = testStack.pop();
  std::cout << "\n  " << pTop->show();
  testStack.top()->endLineCount = 75;
  pTop = testStack.pop();
  std::cout << "\n  " << pTop->show() << "\n" << "\n  Walking simulated AST tree:" << "\n -----------------------------\n\n";
}

#endif
