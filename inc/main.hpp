#pragma once
#define __mountpoint "/tmp/mountpoint"
#ifndef PLATFORM
#error "ERROR: macro PALTFORM not defined!"
#endif

#include "loadconfig.hpp"
#include "runprog.hpp"
#include "format.hpp"
#include <sys/wait.h>
constexpr const char* mntPoint = __mountpoint;
constexpr const char* cfgPoint = __mountpoint "/.live/" PLATFORM;
using std::string;
// functions
int	 runCMD(const char* cmd);
int	 runCMD(const string& cmd);
bool beginWith(const char* str, const char* head);
bool endWith(const char* str, const char* end);
bool streql(const char* _a, const char* _b);
