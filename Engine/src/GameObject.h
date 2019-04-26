#pragma once
#include "Common.h"
#include "Platform/Optional.h"

struct GameObject {

	int id;
	std::string name;
	Optional<int> holder;
	Optional<std::string> short_desc;
	Optional<std::string> first_time_desc;
	Optional<std::string> desc;
	Optional<int> N, E, W, S, U, D;
	int is_viewed, is_getable;

};