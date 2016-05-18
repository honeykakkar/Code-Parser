#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////
// DataStore.h - Store results of file search                      //
//                                                                 //
// Jim Fawcett, Help Session Demo, Spring 2015					   //
// Last Modified by: Honey Kakkar, Computer Engineering, SU        //
//              hkakkar@syr.edu   
/////////////////////////////////////////////////////////////////////

// ver 1.1 - Mar 18 2016
//  - added isEmpty() method
#include <vector>

namespace Scanner
{
	class DataStore
	{
	public:
		using iterator = std::vector<std::string>::iterator;
		void save(const std::string& filespec) { store.push_back(filespec); }
		void save(const std::string& filename, const std::string& path) {}
		iterator begin() { return store.begin(); }
		iterator end() { return store.end(); }
		bool isEmpty()  // Method to check whether Datastore is empty
		{
			return store.empty();
		}

	private:
		std::vector<std::string> store;
	};
}
#endif
