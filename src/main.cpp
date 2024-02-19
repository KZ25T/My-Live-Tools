#include "main.hpp"

int runCMD(const char* cmd) {
	printf("run %s\n", cmd);
	int ret = system(cmd);
	printf("return %d\n", ret);
	return ret;
}

int main(int argc, const char* argv[]) {
	if (argc != 1) {
		return 0;  // not completed yet
	}
	// run while starting up
	LoadConfig ld;
	if (ld.success()) {
		printf("founded .live!\n");
		runProg(ld.getDir(), "/tmp/mountpoint/.live");
		umount(mountPoint);
	}
	return 0;
}
