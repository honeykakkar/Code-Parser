bool SemiExp::get(bool clear)
{
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	if (clear == true)
		_tokens.clear();
	size_t Colon = 0;
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token == "for")
		{
			_tokens.push_back(token);
			while (true)
			{
				token = _pToker->getTok();
				_tokens.push_back(token);
				if (token == ";")
					++Colon;
				if (Colon == 3 || token == "{")
					return true;
				if (token == ";" && SearchCollection(";;"))
					return true;
			}
		}
		if (token == "#")
		{
			_tokens.push_back(token);
			while (token != "\n")
			{
				token = _pToker->getTok();
				_tokens.push_back(token);
			}
			return true;
		}
		if (token == "")
			break;
		std::string LastToken = "";
		if (!_tokens.empty())
			LastToken = _tokens.back();
		_tokens.push_back(token);
		if ((LastToken == "public" || LastToken == "protected" || LastToken == "private") && token == ":")
			return true;
		if (Colon == 3)
			return true;
		if (token == "{" || token == ";" || token == "}")
			return true;
	}
	return false;
}

