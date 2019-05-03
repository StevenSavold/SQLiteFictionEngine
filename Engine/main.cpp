#include "Common.h"
#include "Parsing/ParseUtils.h"
#include "Database.h"
#include "lua/lua.hpp"


void printUsage(const char* progName);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void printOffscreenDesc(const Database& db);
void DynamicCommandDispatch(lua_State* L, Database& db, const std::pair<Database::ID_type, Database::ID_type>& parsedIDs);

int GetCurrentRoom_Lua(lua_State* L);
int GetRoomTransitions_Lua(lua_State* L);
int PlayerTransitionTo_Lua(lua_State* L);
int PrintLocationText_Lua(lua_State* L);
int PrintOffscreenText_Lua(lua_State* L);
int GetHolder_Lua(lua_State* L);
int GetPlayerID_Lua(lua_State* L);
int IsViewed_Lua(lua_State* L);
int IsGettable_Lua(lua_State* L);
int PrintItemDescription_Lua(lua_State* L);
int PrintFirstDescription_Lua(lua_State* L);
int UpdateHolder_Lua(lua_State* L);

/* Global Database Instance */
Database* g_DBInst = nullptr;


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
		lua_register(L, "get_room_transitions", GetRoomTransitions_Lua);
		lua_register(L, "player_transition_to", PlayerTransitionTo_Lua);
		lua_register(L, "print_location_text", PrintLocationText_Lua);
		lua_register(L, "print_offscreen_text", PrintOffscreenText_Lua);
		lua_register(L, "get_holder", GetHolder_Lua);
		lua_register(L, "get_player_id", GetPlayerID_Lua);
		lua_register(L, "is_viewed", IsViewed_Lua);
		lua_register(L, "is_gettable", IsGettable_Lua);
		lua_register(L, "print_desc", PrintItemDescription_Lua);
		lua_register(L, "print_first_desc", PrintFirstDescription_Lua);
		lua_register(L, "update_holder", UpdateHolder_Lua);

		/* 
	     * Do the assets file provided by the author to perform 
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

		/*
		 * Use the IDs to dispatch the command into Lua
		 */
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
    std::unique_ptr<std::string> desc = db.GetDescription(OFFSCREEN_ID);
    log("%s\n", desc->c_str());
}

void DynamicCommandDispatch(lua_State* L, Database& db, const std::pair<Database::ID_type, Database::ID_type>& parsedIDs)
{
	if (parsedIDs.first < 0)
	{
		// The verb was invalid
		log("I dont know what you mean by that...\n");
		return;
	}

	// Get the verb name from the
	std::string whereClause = "id=";
	whereClause += std::to_string(parsedIDs.first);
	auto funcName = db.GetFrom<std::string>("verbs", "verb", whereClause.c_str(), 
		[](void* output, int argc, char** argv, char** colName) -> int 
	{
		(*(static_cast<std::string*>(output))) += argv[0];
		return 0;
	});

	/* Transform all spaces into '_' */
	std::transform(funcName->begin(), funcName->end(), funcName->begin(), 
		[](unsigned char c) -> unsigned char 
	{ return (c == ' ') ? '_' : c; });

	lua_getglobal(L, funcName->c_str());
	if (lua_isfunction(L, -1))
	{
		// If the function exists. Pass the parameter in and call it.
		lua_pushnumber(L, parsedIDs.second);
		lua_call(L, 1, 0);	
	}
	else
	{
		// If the thing on the top of the stack is not a lua function, error
		err("No lua function exists by the name of '%s'\n", funcName->c_str());
	}
	
}

int GetCurrentRoom_Lua(lua_State* L)
{
	auto player = g_DBInst->GetObject(PLAYER_ID);
	lua_pushnumber(L, player->holder);

	return 1;
}

int GetRoomTransitions_Lua(lua_State* L)
{
	std::unique_ptr<Directions> roomDirections = nullptr;

	// get the thing at the top of the stack
	int roomID = lua_tointeger(L, -1);
	if (roomID >= 0)
	{
		roomDirections = g_DBInst->GetDirections(roomID);
		lua_pushnumber(L, roomDirections->North);
		lua_pushnumber(L, roomDirections->South);
		lua_pushnumber(L, roomDirections->East );
		lua_pushnumber(L, roomDirections->West );
		lua_pushnumber(L, roomDirections->Up   );
		lua_pushnumber(L, roomDirections->Down );
	}

	return 6;

}

bool isValidRoomID(int newRoomID)
{
	if (newRoomID < 0) {
		return false;
	}

	std::string whereClause = "id=";
	whereClause += std::to_string(newRoomID);

	std::unique_ptr<int> query = g_DBInst->GetFrom<int>("object", "id", whereClause.c_str(), 
		[](void* output, int argc, char** argv, char** colName) -> int
	{
		*(static_cast<int*>(output)) = atoi(argv[0]);
		return 0;
	});

	if (*query != newRoomID)
	{
		return false;
	}
	else
	{
		return true;
	}

}

int PlayerTransitionTo_Lua(lua_State* L)
{
	// The thing at the top of the stack is the new room id
	int newRoomID = lua_tointeger(L, -1);

	if (isValidRoomID(newRoomID))
	{
		auto player = g_DBInst->GetObject(PLAYER_ID);
		player->holder = newRoomID;
		g_DBInst->UpdateObject(*player);

		lua_pushboolean(L, 1);
	}
	else
	{
		/* Invalid transition */
		lua_pushboolean(L, 0);
	}
	
	return 1;

}

int PrintLocationText_Lua(lua_State* L)
{

	auto player = g_DBInst->GetObject(PLAYER_ID);
	auto currLocation = g_DBInst->GetObject(player->holder);

	log("You are in the %s\n\n", currLocation->name.c_str());
	log("\t%s\n", currLocation->desc.c_str());

	return 0;

}

int PrintOffscreenText_Lua(lua_State* L)
{
	printOffscreenDesc(*g_DBInst);
	return 0;
}

int GetHolder_Lua(lua_State* L)
{
	// Retrieve the param from lua
	int itemID = lua_tointeger(L, -1);
	// get info about the item
	auto item = g_DBInst->GetObject(itemID);
	// place the return value on the lua stack
	lua_pushinteger(L, item->holder);
	// tell lua how many returns you placed on the stack
	return 1;

}

int GetPlayerID_Lua(lua_State* L)
{
	lua_pushinteger(L, PLAYER_ID);
	return 1;
}

int IsViewed_Lua(lua_State* L)
{
	// Retrieve the param from lua
	int itemID = lua_tointeger(L, -1);
	// get info about the item
	auto item = g_DBInst->GetObject(itemID);
	// place the return value on the lua stack
	lua_pushboolean(L, item->is_viewed);
	// update the item to be viewed if it isnt already
	if (!item->is_viewed)
	{
		item->is_viewed = 1;
		g_DBInst->UpdateObject(*item);
	}
	// tell lua how many returns you placed on the stack
	return 1;
}

int IsGettable_Lua(lua_State* L)
{

	// Retrieve the param from lua
	int itemID = lua_tointeger(L, -1);
	// get info about the item
	auto item = g_DBInst->GetObject(itemID);
	// place the return value on the lua stack
	lua_pushboolean(L, item->is_getable);
	// tell lua how many returns you placed on the stack
	return 1;

}

int PrintItemDescription_Lua(lua_State* L)
{
	// Retrieve the param from lua
	int itemID = lua_tointeger(L, -1);
	
	// get info about the item
	auto item = g_DBInst->GetObject(itemID);

	// print the description
	log("%s\n", item->desc.c_str());

	// return nothing
	return 0;
}

int PrintFirstDescription_Lua(lua_State* L)
{
	// Retrieve the param from lua
	int itemID = lua_tointeger(L, -1);
	
	// get info about the item
	auto item = g_DBInst->GetObject(itemID);

	// print the description
	log("%s\n", item->first_time_desc.c_str());

	// return nothing
	return 0;
}

int UpdateHolder_Lua(lua_State* L)
{
	int itemID = lua_tointeger(L, -2);
	int newHolder = lua_tointeger(L, -1);

	auto item = g_DBInst->GetObject(itemID);
	item->holder = newHolder;
	g_DBInst->UpdateObject(*item);

	return 0;
}