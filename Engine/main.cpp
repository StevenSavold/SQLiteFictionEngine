#include "Common.h"
#include "Database.h"
#include "Parsing/ParseUtils.h"

void printUsage(const char* progName);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void printOffscreenDesc(const Database& db);
std::string GenerateSQLFromIDs(const std::pair<Database::ID_type, Database::ID_type>& ids);
Database::SQLCallbackFn GetSQLCallbackFunction(const std::pair<Database::ID_type, Database::ID_type>& ids, const Database& db);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    Database db(argv[1]);
    if (!db.is_open())
    {
        err("Cannot open database '%s'", argv[1]);
        return EXIT_FAILURE;
    }

    bool shouldQuit = false;
    std::string userInput;
	std::string processedInput;
    std::pair<Database::ID_type, Database::ID_type> parsedIDs;
    std::string command;
    Database::SQLCallbackFn callbackFn;

    //printOffscreenDesc(db);
    //while (!shouldQuit)
    {
        /* 
		 * Returns a copy of the users entered string,
		 * and has side effect of printing "> " to prompt
		 * the user.
		 */
		userInput = GetUserInput();

		/* 
		 * Takes a const reference to the RAW input and returns
		 * a copy of the input preprocessed into a more
		 * manageable form for the game to handle.
		 */
		processedInput = PreprocessInput(userInput);

        /*
         * Parse the preprocessed input to a the IDs of the verb
         * and noun
         */ 
        parsedIDs = ParseInputToSQLCommand(processedInput, db);

		log("{ %d, %d }\n", parsedIDs.first, parsedIDs.second);

        /*
         * Using the IDs, generate a SQL command to perform the 
         * requested action(s) on the database. 
         */
        command = GenerateSQLFromIDs(parsedIDs);

        /*
         * Retrieve the callback function to be used at the time
         * of SQL execution. 
         */
        callbackFn = GetSQLCallbackFunction(parsedIDs, db);

        /*
         * Execute the command on the database. Perform the
         * callback on the results
         */
        //db.Exec(command.c_str(), callbackFn);

    }

    return EXIT_SUCCESS;
}

void printUsage(const char* progName)
{
	log("USAGE: ");
    log("    %s database_file\n", progName);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void printOffscreenDesc(const Database& db)
{
    std::unique_ptr<std::string> desc = db.GetDescription(0);
    log("%s\n", desc->c_str());
}

std::string GenerateSQLFromIDs(const std::pair<Database::ID_type, Database::ID_type>& ids)
{
    return "";
}

Database::SQLCallbackFn GetSQLCallbackFunction(const std::pair<Database::ID_type, Database::ID_type>& ids, const Database& db)
{
    return callback;
}