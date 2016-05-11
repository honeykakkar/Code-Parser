#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "ITokCollection.h"
namespace Scanner
{
	using Token = std::string;
	class SemiExp : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker = nullptr);
		SemiExp(const SemiExp&) = delete;
		SemiExp& operator=(const SemiExp&) = delete;
		bool get(bool clear = true);
		size_t length();
		std::string& operator[](int n);
		size_t find(const std::string& tok);
		void push_back(const std::string& tok);
		bool merge(const std::string& firstTok, const std::string& secondTok);
		bool remove(const std::string& tok);
		bool remove(size_t i);
		void toLower();
		void trimFront();
		void clear();
		bool SearchCollection(std::string T);
		Token operator[](size_t n);
		std::string show(bool showNewLines = false); // Displays vector of tokens on screen
	private:
		std::vector<Token> _tokens;
		Toker* _pToker = nullptr;
	};
}
#endif