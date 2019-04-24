#pragma once

/*
 * Used for std::string, std::vector, and std::transform
 */
#include "Common.h"
#include "Database.h"

std::string GetUserInput();
std::string PreprocessInput(const std::string& rawInput);
std::string ParseInputToSQLCommand(const std::string& preprocessedInput, const Database& db);

std::string FindFirstLongestKeyword(const std::string& searchSpace, const std::vector<std::pair<std::string, Database::DBInt>>& keywords);
std::string FindLastLongestKeyword(const std::string& searchSpace, const std::vector<std::string>& keywords);
