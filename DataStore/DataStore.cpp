/////////////////////////////////////////////////////////////////////
// DataStore.cpp - Store results of file search and contains       //
//                 Test stub                                       //
//                                                                 //
// Jim Fawcett, Help Session Demo, Spring 2015                     //
/////////////////////////////////////////////////////////////////////

#include "DataStore.h"
#include <iostream>

// Test Stub
#ifdef TEST_DATASTORES
using namespace Scanner;

int main()
{
  std::cout << "\n  Testing DataStore";

  DataStore ds;
  ds.save("one");
  ds.save("two");
  ds.save("three");
  DataStore::iterator iter = ds.begin();
  std::cout << "\n  " << (*iter).c_str();

  for (auto item : ds)
  {
    std::cout << "\n  " << item.c_str();
  }
  std::cout << "\n\n";
}

#endif
