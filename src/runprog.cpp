#include "runprog.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
/**
 * @brief search and joint add deb paths
 *
 * @param path deb dir path
 * @return string deb paths
 */
static string findDEB(const char* path) {
	DIR*	dir	  = opendir(path);
	dirent* entry = nullptr;
	string	pkgs  = "";
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_type == DT_REG && endWith(entry->d_name, ".deb") == true) {
			pkgs += format("{}/{} ", path, entry->d_name);
		}
	}
	if (pkgs.size() != 0) pkgs.erase(pkgs.end() - 1);
	return pkgs;
}

/**
 * @brief run basic progs and delete prog
 *
 * @param dirPtr dir ptr
 * @param dirPath dir name
 */
void runProg(DIR* dirPtr, const char* dirPath, bool redirect) {
	// process args
	bool releaseDirPath = false;
	if (dirPtr == nullptr && dirPath == nullptr) return;
	if (dirPtr == nullptr) {
		dirPtr = opendir(dirPath);
		if (dirPath == nullptr) return;
	}
	// if endWith with '/', new some space to direct to a new place
	if (dirPath != nullptr && endWith(dirPath, "/") == true) {
		releaseDirPath = true;
		size_t size	   = strlen(dirPath);
		char*  tmp	   = new char[size];
		memcpy(tmp, dirPath, size);
		tmp[size - 1] = 0;
		dirPath		  = tmp;
	}
	// after process args
	// judge which option to run
	dirent* entry	   = nullptr;
	bool	RunPreScr  = false;
	bool	RunOverlay = false;
	bool	RunDEBPack = false;
	bool	RunPostScr = false;
	while ((entry = readdir(dirPtr)) != nullptr) {
		if (entry->d_type == DT_REG && strcmp(entry->d_name, "preScript") == 0) {
			RunPreScr = true;
		}
		if (entry->d_type == DT_REG && strcmp(entry->d_name, "overlay.zip") == 0) {
			RunOverlay = true;
		}
		if (entry->d_type == DT_DIR && strcmp(entry->d_name, "packages") == 0) {
			RunDEBPack = true;
		}
		if (entry->d_type == DT_REG && strcmp(entry->d_name, "postScript") == 0) {
			RunPostScr = true;
		}
	}
	closedir(dirPtr);
	// run those options
	if (RunPreScr == true) {
		// run startup scripts
		const char* log = redirect ? ">/var/log/mlt.1.log 2>&1" : "";
		string		cmd = format("bash {}/preScript {}", dirPath, log);
		runCMD(cmd);
	}
	if (RunOverlay == true) {
		// unzip rootfs target
		const char* log = redirect ? ">/var/log/mlt.2.log 2>&1" : "";
		string		cmd = format("unzip -o {}/overlay.zip -d / {}", dirPath, log);
		runCMD(cmd);
	}
	if (RunDEBPack == true) {
		// install deb
		const char* log		= redirect ? ">/var/log/mlt.3.log 2>&1" : "";
		string		debpath = dirPath + string("/packages");
		string		debpkgs = findDEB(debpath.c_str());
		// have some deb packs
		if (debpkgs.size() != 0) {
			string cmd = format("dpkg -i {} {}", debpkgs, log);
			runCMD(cmd);
		}
	}
	if (RunPostScr == true) {
		// run startup scripts
		const char* log = redirect ? ">/var/log/mlt.4.log 2>&1" : "";
		string		cmd = format("bash {}/postScript {}", dirPath, log);
		runCMD(cmd);
	}
	if (releaseDirPath == true) {
		delete[] dirPath;
		dirPath = nullptr;
	}
	return;
}
