#pragma once

#include "Common.h"
#include "sqlite3/sqlite3.h"
#include <cstdarg>
#include <string>

#define OFFSCREEN_ID 0
#define PLAYER_ID 1


union Directions {
    int dirs[6];
    struct {
		int North, South, East, West, Up, Down;
    };
};

class Database {
private: 
    sqlite3* db_;

public:

    using SQLCallbackFn = int(*)(void*, int, char**, char**);
    using ID_type = int;
	using DBInt = int;

    Database(const char* dbfile)
    {
        int rc = sqlite3_open(dbfile, &db_);

        // If we fail to make the db then make the db a nullptr
        if (rc)
        {
            sqlite3_close(db_);
            db_ = nullptr;
        }
    }

    ~Database() 
    {
        sqlite3_close(db_);
        db_ = nullptr;
    }

    bool is_open() 
    {
        return (db_ != nullptr);
    }

    int Exec(const char* sql_code, SQLCallbackFn callbk)
    {
        char* errMsg;
        int rc = sqlite3_exec(db_, sql_code, callbk, 0, &errMsg);
        
        if (rc != SQLITE_OK)
        {
            err("SQL ERROR: %s\n", errMsg);
            sqlite3_free(errMsg);
        }

        return rc;
    }

    template <typename T>
    std::unique_ptr<T> GetFrom(const char* tableName, const char* colNames, SQLCallbackFn callbk) const
    {
        char* errMsg;
        std::unique_ptr<T> output = std::make_unique<T>();

        std::string code;
        code += "SELECT ";
        code += colNames;
        code += " FROM ";
        code += tableName;

        int rc = sqlite3_exec(db_, code.c_str(), callbk, output.get(), &errMsg);
        if (rc != SQLITE_OK)
        {
            err("SQL ERROR: %s\n", errMsg);
            sqlite3_free(errMsg);
            return nullptr;
        }

        return std::move(output);
    }

	template <typename T>
	std::unique_ptr<T> GetFrom(const char* tableName, const char* colNames, const char* whereCondition, SQLCallbackFn callbk) const
	{
		char* errMsg;
		std::unique_ptr<T> output = std::make_unique<T>();

		std::string code;
		code += "SELECT ";
		code += colNames;
		code += " FROM ";
		code += tableName;
		code += " WHERE ";
		code += whereCondition;

		int rc = sqlite3_exec(db_, code.c_str(), callbk, output.get(), &errMsg);
		if (rc != SQLITE_OK)
		{
			err("SQL ERROR: %s\n", errMsg);
			sqlite3_free(errMsg);
			return nullptr;
		}

		return std::move(output);
	}

    std::unique_ptr<std::string> GetDescription(ID_type itemId) const 
    {
		std::string whereClause = "id=";
		whereClause += std::to_string(itemId);

		return std::move(GetFrom<std::string>("object", "desc", whereClause.c_str(), 
			[](void* output, int argc, char** argv, char** colName) -> int {
				if (argc > 1)
				{
					err("Too many columns!\n");
					return 1;
				}

				(*(static_cast<std::string*>(output))) += argv[0];

				return 0;
		}));
    }

    std::unique_ptr<Directions> GetDirections(ID_type itemId) const 
    {
		std::string whereClause = "id=";
		whereClause += std::to_string(itemId);

		return std::move(GetFrom<Directions>("object", "N, S, E, W, U, D", whereClause.c_str(),
			[](void* output, int argc, char** argv, char** colName) -> int {
				if (argc > 6)
				{
					err("Too many columns!\n");
					return 1;
				}

				for (int i = 0; i < argc; ++i)
				{
					static_cast<Directions*>(output)->dirs[i] = atoi((argv[i] != nullptr) ? argv[i] : "-1");
				}

				return 0;
		}));
    }

	std::unique_ptr<GameObject> GetObject(ID_type objID) const {

		std::string code = "id=";
		code += std::to_string(objID);

		return std::move(GetFrom<GameObject>("object", "*", code.c_str(),
			[](void* output, int argc, char** argv, char** colNames) -> int
		{

			GameObject* object = static_cast<GameObject*>(output);
			object->id = atoi(argv[0]);
			object->name = argv[1];
			object->holder = SafeSetInt(argv[2]);
			object->short_desc = SafeSetString(argv[3]);
			object->first_time_desc = SafeSetString(argv[4]);
			object->desc = SafeSetString(argv[5]);
			object->N = SafeSetInt(argv[6]);
			object->E = SafeSetInt(argv[7]);
			object->W = SafeSetInt(argv[8]);
			object->S = SafeSetInt(argv[9]);
			object->U = SafeSetInt(argv[10]);
			object->D = SafeSetInt(argv[11]);
			object->is_viewed = atoi(argv[12]);
			object->is_getable = atoi(argv[13]);
			return 0;

		}));
	}


	void UpdateObject(const GameObject& obj)
	{
		std::string sqlCode = "UPDATE object SET id=";
		sqlCode += std::to_string(obj.id);
		sqlCode += ", name='";
		sqlCode += obj.name;
		sqlCode += "', holder=";
		sqlCode += SafeAccessInt(obj.holder);
		sqlCode += ", short_desc=";
		sqlCode += SafeAccessString(obj.short_desc);
		sqlCode += ", first_time_desc=";
		sqlCode += SafeAccessString(obj.first_time_desc);
		sqlCode += ", desc="; 
		sqlCode += SafeAccessString(obj.desc);
		sqlCode += ", N=";
		sqlCode += SafeAccessInt(obj.N);
		sqlCode += ", E=";
		sqlCode += SafeAccessInt(obj.E);
		sqlCode += ", W=";
		sqlCode += SafeAccessInt(obj.W);
		sqlCode += ", S=";
		sqlCode += SafeAccessInt(obj.S);
		sqlCode += ", U=";
		sqlCode += SafeAccessInt(obj.U);
		sqlCode += ", D=";
		sqlCode += SafeAccessInt(obj.D);
		sqlCode += ", is_viewed=";
		sqlCode += std::to_string(obj.is_viewed);
		sqlCode += ", is_getable=";
		sqlCode += std::to_string(obj.is_getable);
		sqlCode += " WHERE id=";
		sqlCode += std::to_string(obj.id);

		Exec(sqlCode.c_str(), 
			[](void* output, int argc, char** argv, char** colNames) -> int
		{ /* DO NOTHING, ONLY EXEC THE STATEMENT */ return 0; });

	}


};