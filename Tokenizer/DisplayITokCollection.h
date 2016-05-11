#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
#include <string>
namespace Scanner
{
	struct ITokCollection
	{
		virtual bool get(bool clear = true) = 0;
		virtual size_t length() = 0;
		virtual std::string& operator[](int n) = 0;
		virtual size_t find(const std::string& tok) = 0;
		virtual void push_back(const std::string& tok) = 0;
		virtual bool merge(const std::string& firstTok, const std::string& secondTok) = 0;
		virtual bool remove(const std::string& tok) = 0;
		virtual bool remove(size_t i) = 0;
		virtual void toLower() = 0;
		virtual void trimFront() = 0;
		virtual void clear() = 0;
		virtual std::string show(bool showNewLines = false) = 0;
		virtual ~ITokCollection() {};
	};
}
#endif