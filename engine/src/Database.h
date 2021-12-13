#pragma once

#include "Common.h"
#include "Parsing/ParseUtils.h"
#include "sqlite3.h"
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
			object->holder = (argv[2] != nullptr) ? atoi(argv[2]) : -1;
			object->short_desc = (argv[3] != nullptr) ? argv[3] : "";
			object->first_time_desc = (argv[4] != nullptr) ? argv[4] : "";
			object->desc = (argv[5] != nullptr) ? argv[5] : "";
			object->N = (argv[6] != nullptr) ? atoi(argv[6]) : -1;
			object->E = (argv[7] != nullptr) ? atoi(argv[7]) : -1;
			object->W = (argv[8] != nullptr) ? atoi(argv[8]) : -1;
			object->S = (argv[9] != nullptr) ? atoi(argv[9]) : -1;
			object->U = (argv[10] != nullptr) ? atoi(argv[10]) : -1;
			object->D = (argv[11] != nullptr) ? atoi(argv[11]) : -1;
			object->is_viewed = atoi(argv[12]);
			object->is_getable = atoi(argv[13]);
			return 0;

		}));
	}

	void UpdateObject(const GameObject& obj)
	{
		std::string sqlCode = "UPDATE object SET id=";
		sqlCode += std::to_string(obj.id);
		sqlCode += ", name=";
		sqlCode += SafeWrap(obj.name);
		sqlCode += ", holder=";
		sqlCode += (obj.holder != -1) ? std::to_string(obj.holder) : "NULL";
		sqlCode += ", short_desc=";
		sqlCode += (obj.short_desc.empty()) ? "NULL" : SafeWrap(obj.short_desc).c_str();
		sqlCode += ", first_time_desc=";
		sqlCode += (obj.first_time_desc.empty()) ? "NULL" : SafeWrap(obj.first_time_desc).c_str();
		sqlCode += ", desc="; 
		sqlCode += (obj.desc.empty()) ? "NULL" : SafeWrap(obj.desc).c_str();
		sqlCode += ", N=";
		sqlCode += (obj.N != -1) ? std::to_string(obj.N) : "NULL";
		sqlCode += ", E=";
		sqlCode += (obj.E != -1) ? std::to_string(obj.E) : "NULL";
		sqlCode += ", W=";
		sqlCode += (obj.W != -1) ? std::to_string(obj.W) : "NULL";
		sqlCode += ", S=";
		sqlCode += (obj.S != -1) ? std::to_string(obj.S) : "NULL";
		sqlCode += ", U=";
		sqlCode += (obj.U != -1) ? std::to_string(obj.U) : "NULL";
		sqlCode += ", D=";
		sqlCode += (obj.D != -1) ? std::to_string(obj.D) : "NULL";
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