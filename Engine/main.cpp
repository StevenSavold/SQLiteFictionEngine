#include "Common.h"
#include "Database.h"
#include "Systems/VerbRegistery.h"
#include "Parsing/ParseUtils.h"

void printUsage(const char* progName);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void printOffscreenDesc(const Database& db);
std::unique_ptr<VerbRegistry> RegisterVerbs(const Database& db);

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

	//Initialize Systems
	std::unique_ptr<VerbRegistry> vr = RegisterVerbs(db); //Create the verb registry


    bool shouldQuit = false;
    std::string userInput;
	std::string processedInput;
	std::pair<Database::ID_type, Database::ID_type> parsedIDs;
	VerbFn command;
	

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

        /*
         * Retrieve the command function to be used at the time
         * of SQL execution. 
         */
        command = vr->GetVerbFunction(parsedIDs.first);

        /*
         * Perform the command, giving it the parsed id for the
		 * object
         */
		command(db, parsedIDs.second);

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

std::unique_ptr<VerbRegistry> RegisterVerbs(const Database& db)
{
	std::unique_ptr<VerbRegistry> vr = std::make_unique<VerbRegistry>();

	auto verbs = db.GetFrom<std::vector<std::pair<std::string, Database::ID_type>>>("verbs", "verb, verb_id",
		[](void* output, int argc, char** argv, char** colName) -> int
	{
		std::vector<std::pair<std::string, Database::ID_type>>* verbList = static_cast<std::vector<std::pair<std::string, Database::ID_type>>*>(output);
		verbList->emplace_back(argv[0], atoi(argv[1]));

		return 0;
	});

	for (const auto& verb : *verbs)
	{
		vr->RegisterVerb(verb.second, [verb](Database& db, Database::ID_type objID) {
		
			/* 
			 * Idea for future implementation:
			 *     Use lua to call a lua function with
			 *     the same name as the verb to do this 
			 *     stuff.
			 */

			/* Get the player info */
			auto player = db.GetObject(PLAYER_ID);

			/* Get the directions of the players holder */
			auto directions = db.GetDirections(*(player->holder));

			player->holder = (directions->East >= 0) ? (Optional(directions->East)) : Nullopt ;

			db.UpdateObject(*player);

			auto newPlace = db.GetObject(*(player->holder));

			log("player is now at %s\n", newPlace->name.c_str());

		});
	}

	return std::move(vr);
}