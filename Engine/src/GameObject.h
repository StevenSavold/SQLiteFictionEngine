#pragma once
#include "Common.h"
#include "Platform/Optional.h"

struct GameObject {

	int id;
	std::string name;
	int holder;
	std::string short_desc;
	std::string first_time_desc;
	std::string desc;
	int N, E, W, S, U, D;
	int is_viewed, is_getable;

};