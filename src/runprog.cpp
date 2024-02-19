#include "runprog.hpp"
#include "main.hpp"
#include <format>
#include <stdio.h>
#include <stdlib.h>
#include <string>
/**
 * @brief judge whether str is endup with end
 *
 * @param str
 * @param end
 * @return true
 * @return false
 */
static bool endup(const char* str, const char* end) {
	int L_str = strlen(str);
	int L_end = strlen(end);
	if (strcmp(str + (L_str - L_end), end) == 0)
		return true;
	else
		return false;
};
/**
 * @brief search and joint add deb paths
 *
 * @param path deb dir path
 * @return std::string deb paths
 */
static std::string findDEB(const char* path) {
	DIR*		dir	  = opendir(path);
	dirent*		entry = nullptr;
	std::string ret	  = "";
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_type == DT_REG && endup(entry->d_name, ".deb") == true) {
			ret += std::format("{}/{} ", path, entry->d_name);
		}
	}
	return ret;
}

/**
 * @brief run basic progs
 *
 * @param dirPtr dir ptr
 * @param dirPath dir name
 */
void runProg(DIR* dirPtr, const char* dirPath) {
	if (dirPtr == nullptr) return;
	// if endup with '/', new some space to direct to a new place
	bool shouldRelease = false;
	if (dirPath != nullptr && endup(dirPath, "/") == true) {
		shouldRelease = true;
		size_t size	  = strlen(dirPath);
		char*  tmp	  = new char[size];
		memcpy(tmp, dirPath, size);
		tmp[size - 1] = 0;
		dirPath		  = tmp;
	}
	dirent* entry	   = nullptr;
	bool	RunOverlay = false;
	bool	RunDEBPack = false;
	bool	RunScripts = false;
	while ((entry = readdir(dirPtr)) != nullptr) {
		if (entry->d_type == DT_REG && strcmp(entry->d_name, "overlay.zip") == 0) {
			RunOverlay = true;
		}
		if (entry->d_type == DT_DIR && strcmp(entry->d_name, "packages") == 0) {
			RunDEBPack = true;
		}
		if (entry->d_type == DT_REG && strcmp(entry->d_name, "startup-scripts") == 0) {
			RunScripts = true;
		}
	}
	closedir(dirPtr);
	if (RunOverlay == true) {
		// unzip rootfs target
		std::string cmd = std::format("unzip -o {}/overlay.zip -d /", dirPath);
		runCMD(cmd.c_str());
	}
	if (RunDEBPack == true) {
		// install deb
		std::string debpath = dirPath + std::string("/packages");
		std::string cmd		= std::format("dpkg -i {}", findDEB(debpath.c_str()));
		runCMD(cmd.c_str());
	}
	if (RunScripts == true) {
		// run startup scripts
		std::string cmd = std::format("bash {}/startup-scripts", dirPath);
		runCMD(cmd.c_str());
	}
	if (shouldRelease == true) delete[] dirPath;
	return;
}
