#include "Common.h"
#include "Database.h"
#include "Parsing/ParseUtils.h"
#include "lua/lua.hpp"


void printUsage(const char* progName);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void printOffscreenDesc(const Database& db);
void DynamicCommandDispatch(lua_State* L, Database& db, const std::pair<Database::ID_type, Database::ID_type>& parsedIDs);

int GetCurrentRoom_Lua(lua_State* L);


/* Global Database Instance */
Database* g_DBInst = nullptr;


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

	log("%d\n", sizeof(int) == sizeof(Optional<int>));


    Database db(argv[1]);
    if (!db.is_open())
    {
        err("Cannot open database '%s'", argv[1]);
        return EXIT_FAILURE;
    }
	g_DBInst = &db;

	//-- Initialize Systems --//

	/* Start Lua Init */
	lua_State* L = luaL_newstate();
	if (L == nullptr)
	{
		// something went wrong
		err("Could not create lua state!\n");
		return EXIT_FAILURE;
	}
	else 
	{
		/* Open the lua std libs for use */
		luaL_openlibs(L);

		/* Register engine functions with lua for use */
		lua_register(L, "get_current_room", GetCurrentRoom_Lua);

		/* 
	     * Do the assests file provided by the author to perform 
		 * init code and load the verb function symbols.
		 */
		if (luaL_dofile(L, "res/assets.lua"))
		{
			err("assests.lua could not be loaded!\n");
		}
	}
	/* End Lua Init */


    bool shouldQuit = false;
    std::string userInput;
	std::string processedInput;
	std::pair<Database::ID_type, Database::ID_type> parsedIDs;

    //printOffscreenDesc(db);
    while (!shouldQuit)
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

		//log("{ %d, %d }\n", parsedIDs.first, parsedIDs.second);

		DynamicCommandDispatch(L, db, parsedIDs);

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

void DynamicCommandDispatch(lua_State* L, Database& db, const std::pair<Database::ID_type, Database::ID_type>& parsedIDs)
{
	// Get the verb name from the database
	std::string whereClause = "id=";
	whereClause += std::to_string(parsedIDs.first);
	auto funcName = db.GetFrom<std::string>("verbs", "verb", whereClause.c_str(), 
		[](void* output, int argc, char** argv, char** colName) -> int 
	{
		(*(static_cast<std::string*>(output))) += argv[0];
		return 0;
	});

	//log("verb is: %s\n", funcName->c_str());

	lua_getglobal(L, funcName->c_str());
	if (!lua_isfunction(L, -1))
	{
		// If the thing on the top of the stack is not a lua function, error
		err("No lua function exists by the name of '%s'\n", funcName->c_str());

	}
	else
	{
		// If the function exists. call it.
		lua_call(L, 0, 0);
	}
	
}

int GetCurrentRoom_Lua(lua_State* L)
{
	auto player = g_DBInst->GetObject(PLAYER_ID);

	lua_pushnumber(L, player->holder);

	return 1;

}