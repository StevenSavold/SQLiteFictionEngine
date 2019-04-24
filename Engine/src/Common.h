#pragma once

#include <iostream>
#include <cstdio>
#include <memory>
#include <vector>
#include <algorithm>
#include <utility>

#define log(...) \
    fprintf(stdout, __VA_ARGS__)

#define err(...) \
    fprintf(stderr, "ERROR: "); \
    fprintf(stderr, __VA_ARGS__)