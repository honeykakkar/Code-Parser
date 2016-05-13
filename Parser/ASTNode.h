#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace Scanner
{
	struct element
	{
		std::string type;
		std::string name;
		size_t startLineCount;
		size_t endLineCount;
		//size_t Complexity = 1;
		std::vector<element*> _children = {};
		std::string show()
		{
			std::ostringstream temp;
			temp << "(";
			temp << type;
			temp << ", ";
			temp << name;
			temp << ", ";
			temp << startLineCount;
			temp << ", ";
			temp << endLineCount - startLineCount + 1;;
			temp << ")";
			return temp.str();
		}
		std::vector<element*>& GetChildren()
		{
			return (this->_children);
		}

		void AddChild(element* Child)
		{
			this->_children.push_back(Child);
		}

		bool IfEqual(element B)
		{
			return ((name == B.name) && (type == B.type) && (startLineCount = B.startLineCount) && (endLineCount == B.endLineCount));
		}
	};
}