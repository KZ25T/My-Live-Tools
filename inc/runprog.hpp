#pragma once
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "main.hpp"

void runProg(DIR* dirPtr, const char* dirPath, bool redirect = false);
