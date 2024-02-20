#include "runprog.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
using std::format;
/**
 * @brief search and joint add deb paths
 *
 * @param path deb dir path
 * @return string deb paths
 */
static string findDEB(const char* path) {
	DIR*		dir	  = opendir(path);
	dirent*		entry = nullptr;
	string pkgs  = "";
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_type == DT_REG && endWith(entry->d_name, ".deb") == true) {
			pkgs += format("{}/{} ", path, entry->d_name);
		}
	}
	return pkgs;
}

/**
 * @brief run basic progs
 *
 * @param dirPtr dir ptr
 * @param dirPath dir name
 */
void runProg(DIR* dirPtr, const char* dirPath, bool redirect) {
	if (dirPtr == nullptr && dirPath == nullptr) return;
	if (dirPtr == nullptr) {
		dirPtr = opendir(dirPath);
		if (dirPath == nullptr) return;
	}
	// if endWith with '/', new some space to direct to a new place
	bool shouldRelease = false;
	if (dirPath != nullptr && endWith(dirPath, "/") == true) {
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
		const char* log = redirect ? "2>&1 >/var/log/mlt.1.log" : "";
		string cmd = format("unzip -o {}/overlay.zip -d / {}", dirPath, log);
		runCMD(cmd);
	}
	if (RunDEBPack == true) {
		// install deb
		const char* log		= redirect ? "2>&1 >/var/log/mlt.2.log" : "";
		string debpath = dirPath + string("/packages");
		string cmd =
			format("dpkg -i {} {}", findDEB(debpath.c_str()), log);
		runCMD(cmd);
	}
	if (RunScripts == true) {
		// run startup scripts
		const char* log = redirect ? "2>&1 >/var/log/mlt.3.log" : "";
		string cmd = format("bash {}/startup-scripts {}", dirPath, log);
		runCMD(cmd);
	}
	if (shouldRelease == true) delete[] dirPath;
	return;
}
