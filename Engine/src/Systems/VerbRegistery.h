#pragma once
#include "Common.h"
#include "Database.h"


using VerbFn = std::function<void(Database&, Database::ID_type)>;

class VerbRegistry {
private:

	std::unordered_map<Database::ID_type, VerbFn> verbCallbackRegistry_;

public:

	/* Registers a verb (by ID) in the registry. */
	bool RegisterVerb(Database::ID_type verbID, VerbFn verbCallback)
	{
		/* 
		 * If the verbID does NOT exist in the registry, add it.
		 * otherwise return false (meaning the verb was not registered this time)
		 */
		return 
			(verbCallbackRegistry_.find(verbID) == verbCallbackRegistry_.end()) 
			    && 
			(verbCallbackRegistry_[verbID] = verbCallback);
	}

	/* Get a callback from the registry given a key */
	VerbFn GetVerbFunction(const Database::ID_type& id) const
	{
		// NOTE: at() performs a range check
		return verbCallbackRegistry_.at(id);
	}

	/* Get the size of the registry */
	size_t size() const
	{
		return verbCallbackRegistry_.size();
	}

	/* Check if registry is empty */
	bool empty() const
	{
		return verbCallbackRegistry_.empty();
	}

	/* Empty the Registry */
	void clear() 
	{
		verbCallbackRegistry_.clear();
	}

};