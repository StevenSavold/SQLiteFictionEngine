#pragma once

#include "Common.h"
#include "sqlite3/sqlite3.h"
#include <cstdarg>
#include <string>

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

    using SQLCallbackFn = int(void*, int, char**, char**);
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

    int Exec(const char* sql_code, int (*callback)(void*, int, char**, char**))
    {
        char* errMsg;
        int rc = sqlite3_exec(db_, sql_code, callback, 0, &errMsg);
        
        if (rc != SQLITE_OK)
        {
            err("SQL ERROR: %s\n", errMsg);
            sqlite3_free(errMsg);
        }

        return rc;
    }

    template <typename T>
    std::unique_ptr<T> GetColumns(int itemID, const char* colNames, SQLCallbackFn callbk) const
    {
        char* errMsg;
        std::unique_ptr<T> output = std::make_unique<T>();

        std::string code;
        code += "SELECT ";
        code += colNames;
        code += " FROM object WHERE id=";
        code += std::to_string(itemID);

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

    std::unique_ptr<std::string> GetDescription(int itemId) const 
    {
        char* errMsg;
        std::unique_ptr<std::string> output = std::make_unique<std::string>();

        std::string code = "SELECT desc FROM object WHERE id=";
        code += std::to_string(itemId);

        auto callbk = [](void* output, int argc, char** argv, char** colName) -> int {
            if (argc > 1)
            {
                //error
                err("Too many columns!\n");
                return 1;
            }

            (*(static_cast<std::string*>(output))) += argv[0];

            return 0;
        };

        int rc = sqlite3_exec(db_, code.c_str(), callbk, output.get(), &errMsg);
        if (rc != SQLITE_OK)
        {
            err("SQL ERROR: %s\n", errMsg);
            sqlite3_free(errMsg);
            return nullptr;
        }

        return std::move(output);
    }

    std::unique_ptr<Directions> GetDirections(int itemId) const 
    {
        char* errMsg;
        std::unique_ptr<Directions> output = std::make_unique<Directions>();

        std::string code = "SELECT N, S, E, W, U, D FROM object WHERE id=";
        code += std::to_string(itemId);

        auto callbk = [](void* output, int argc, char** argv, char** colName) -> int {
            if (argc > 6)
            {
                //error
                err("Too many columns!\n");
                return 1;
            }

            for (int i = 0; i < argc; ++i)
            {
                static_cast<Directions*>(output)->dirs[i] = atoi((argv[i] != nullptr) ? argv[i] : "");
            }

            return 0;
        };

        int rc = sqlite3_exec(db_, code.c_str(), callbk, output.get(), &errMsg);
        if (rc != SQLITE_OK)
        {
            err("SQL ERROR: %s\n", errMsg);
            sqlite3_free(errMsg);
            return nullptr;
        }

        return std::move(output);
    }

};