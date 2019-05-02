#pragma once

/*
 * Used for std::string, std::vector, and std::transform
 */
#include "Common.h"
std::string SafeWrap(const std::string& input);

#include "Database.h"

std::string GetUserInput();
std::string PreprocessInput(const std::string& rawInput);
std::pair<Database::ID_type, Database::ID_type> ParseInputToSQLCommand(const std::string& preprocessedInput, const Database& db);

const std::pair<std::string, Database::ID_type>* FindFirstLongestKeyword(const std::string& searchSpace, const std::vector<std::pair<std::string, Database::ID_type>>& keywords);
const std::pair<std::string, Database::ID_type>* FindLastLongestKeyword(const std::string& searchSpace, const std::vector<std::pair<std::string, Database::ID_type>>& keywords);

