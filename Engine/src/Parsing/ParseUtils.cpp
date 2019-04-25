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

std::pair<Database::ID_type, Database::ID_type> ParseInputToSQLCommand(const std::string& preprocessedInput, const Database& db)
{
	std::pair<Database::ID_type, Database::ID_type> parsedIDs = { -1, -1 };

	// make a copy of the input to modify
	std::string current = preprocessedInput; 

	// Get all verbs from the Database
	/* #TODO: verbs table will be changed to a 'verb_syn' table of sorts */
	auto verbs = db.GetFrom<std::vector<std::pair<std::string, Database::ID_type>>>("verbs", "verb, id", 
		[](void* output, int argc, char** argv, char** colName) -> int 
	{
		std::vector<std::pair<std::string, Database::ID_type>>* verbList = static_cast<std::vector<std::pair<std::string, Database::ID_type>>*>(output);
		verbList->emplace_back(argv[0], atoi(argv[1]));

		return 0;
	});

	// Get all object names from the Database
	auto objects = db.GetFrom<std::vector<std::pair<std::string, Database::ID_type>>>("object", "name, id", 
		[](void* output, int argc, char** argv, char** colName) -> int 
	{
		std::vector<std::pair<std::string, Database::ID_type>>* objectList = static_cast<std::vector<std::pair<std::string, Database::ID_type>>*>(output);
		objectList->emplace_back(argv[0], atoi(argv[1]));

		return 0;
	});

	/*
	 * Because of the grammar of the commands the first piece 
	 * of the input string should be a verb. As such, find the
	 * longest first occurrence of any of the keywords in the DB
	 */
	const std::pair<std::string, Database::ID_type>* const foundVerb = FindFirstLongestKeyword(current, *verbs);

	if (foundVerb != nullptr)
	{
		// If the foundVerb variable is found
		// set the output appropriately
		parsedIDs.first = foundVerb->second;

		// we found a verb that matches. so remove the verb from the string
		current.erase(0, foundVerb->first.length());
	}

	

	/* 
	 * Because of the grammar of the commands the last piece 
 	 * of the input should be the noun (aka the object). As such,
	 * find the longest last occurrence of any of the objects in the DB
	 */
	const std::pair<std::string, Database::ID_type>* const foundObject = FindLastLongestKeyword(current, *objects);
	if (foundObject != nullptr)
	{
		// If the foundVerb variable is found
		// set the output appropriately
		parsedIDs.second = foundObject->second;
	}

	// Return the parsed pair of IDs (first being the verb, second being the object)
	// if any are -1 then no vaild ID was found
	return parsedIDs;

}

const std::pair<std::string, Database::ID_type>* FindFirstLongestKeyword(const std::string& searchSpace, const std::vector<std::pair<std::string, Database::ID_type>>& keywords)
{ 
	const std::pair<std::string, Database::ID_type>* foundString = nullptr;
	for (size_t i = 0; i < keywords.size(); ++i)
	{
		size_t pos;
		size_t length = 0;

		if (
			((pos = searchSpace.find(keywords[i].first)) != std::string::npos)
			&&
			(pos == 0)
			&&
			(keywords[i].first.length() > length)
			&& 
			(searchSpace[keywords[i].first.length()] == ' ')
			)
		{
			// If you find something valid, and it starts at position 0
			// and its longer then length, and it ends with a word boundary
			// update our results
			foundString = &keywords[i];
			length = keywords[i].first.length();
		}
	}

	return foundString;
}

const std::pair<std::string, Database::ID_type>* FindLastLongestKeyword(const std::string& searchSpace, const std::vector<std::pair<std::string, Database::ID_type>>& keywords)
{
	const std::pair<std::string, Database::ID_type>* foundString = nullptr;
	for (size_t i = 0; i < keywords.size(); ++i)
	{
		size_t pos;
		size_t length = 0;
		if (
			((pos = searchSpace.rfind(keywords[i].first)) != std::string::npos)
			&&
			(pos + keywords[i].first.length() == searchSpace.length())
			&&
			(keywords[i].first.length() > length)
			)
		{
			// If you find something valid, and it is the last thing
			// in the string and its longer then length.
			// Update our results
			foundString = &keywords[i];
			length = keywords[i].first.length();
		}
	}

	return foundString;
}