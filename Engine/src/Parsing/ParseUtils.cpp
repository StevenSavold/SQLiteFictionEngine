#include "ParseUtils.h"

std::string GetUserInput()
{
	std::string rawInput;
	/* make prompt for user */
	std::cout << "> ";
	/* get RAW user input up to '\n' and discard the '\n' */
	std::getline(std::cin, rawInput);

	return rawInput;
}

std::string PreprocessInput(const std::string& rawInput)
{
	/* Make a copy of the RAW input data to manipulate */
	std::string output = rawInput;

	/* Convert the whole string to lower case */
	std::transform(output.begin(), output.end(), output.begin(), ::tolower);

	return output;
}

std::string ParseInputToSQLCommand(const std::string& preprocessedInput, const Database& db)
{
	// Get all verbs from the Database
	/* #TODO: verbs table will be changed to a 'verb_syn' table of sorts */
	auto verbs = db.GetFrom<std::vector<std::pair<std::string, Database::DBInt>>>("verbs", "verb, id", 
		[](void* output, int argc, char** argv, char** colName) -> int 
	{

		std::vector<std::pair<std::string, Database::DBInt>>* verbList = static_cast<std::vector<std::pair<std::string, Database::DBInt>>*>(output);
		verbList->emplace_back(argv[0], atoi(argv[1]));

		return 0;

	});

	// Get all object names from the Database
	auto objects = db.GetFrom<std::vector<std::string>>("object", "name", 
		[](void* output, int argc, char** argv, char** colName) -> int 
	{

		std::vector<std::string>* objectList = static_cast<std::vector<std::string>*>(output);
		objectList->emplace_back(argv[0]);

		return 0;

	});

	// make a copy of the input to modify
	std::string current = preprocessedInput;

	/*
	 * Because of the grammar of the commands the first piece 
	 * of the input string should be a verb. As such, find the
	 * longest first occurrence of any of the keywords in the DB
	 */
	std::string foundVerb = FindFirstLongestKeyword(current, *verbs);
	if (foundVerb.empty())
	{
		// If the foundVerb variable remains empty
		// Then we did not find a known verb.
		// return invalid result

		/* #TODO: return something invalid here */
		return "";
	}

	// we found a verb that matches. so remove the verb from the string
	current.erase(0, foundVerb.length());

	/* 
	 * Because of the grammar of the commands the last piece 
 	 * of the input should be the noun (aka the object). As such,
	 * find the longest last occurrence of any of the objects in the DB
	 */
	std::string foundObject = FindLastLongestKeyword(current, *objects);
	if (foundVerb.empty())
	{
		// If the foundObject variable remains empty
		// Then we did not find a known verb.
		// return invalid result

		/* #TODO: return something invalid here */
		return "";
	}

	

	return foundVerb;

}

std::string FindFirstLongestKeyword(const std::string& searchSpace, const std::vector<std::pair<std::string, Database::DBInt>>& keywords)
{ 
	std::string foundString;
	for (size_t i = 0; i < keywords.size(); ++i)
	{
		size_t pos;
		size_t length = 0;

		//pos = searchSpace.find(keywords[i].first, keywords[i].first.length());
		//bool x1 = (pos != std::string::npos);

		if (
			((pos = searchSpace.find(keywords[i].first)) != std::string::npos)
			&&
			(pos == 0)
			&&
			(keywords[i].first.length() > length)
			)
		{
			// If you find something valid, and it starts at position 0
			// and its longer then length
			// update our results
			foundString = keywords[i].first;
			length = keywords[i].first.length();
		}
	}
	if (foundString.empty())
	{
		// If the foundString variable remains empty
		// Then we did not find a known keyword.
		// return invalid result (aka empty string)
		return "";
	}
	// Otherwise return the found string
	return foundString;
}

std::string FindLastLongestKeyword(const std::string& searchSpace, const std::vector<std::string>& keywords)
{
	std::string foundString;
	for (size_t i = 0; i < keywords.size(); ++i)
	{
		size_t pos;
		size_t length = 0;
		if (
			((pos = searchSpace.rfind(keywords[i])) != std::string::npos)
			&&
			(pos + keywords[i].length() == searchSpace.length())
			&&
			(keywords[i].length() > length)
			)
		{
			// If you find something valid, and it is the last thing
			// in the string and its longer then length.
			// Update our results
			foundString = keywords[i];
			length = keywords[i].length();
		}
	}
	if (foundString.empty())
	{
		// If the foundString variable remains empty
		// Then we did not find a known keyword.
		// return invalid result (aka empty string)
		return "";
	}
	//Otherwise return the found string
	return foundString;
}