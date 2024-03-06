#include "main.hpp"
bool streql(const char* _a, const char* _b) {
	if (strcmp(_a, _b) == 0)
		return true;
	else
		return false;
}

bool beginWith(const char* _str, const char* head) {
	while (*head != 0) {
		if (*_str != *head) return false;
		_str++;
		head++;
	}
	return true;
}

bool endWith(const char* _str, const char* end) {
	int L_str = strlen(_str);
	int L_end = strlen(end);
	if (streql(_str + (L_str - L_end), end))
		return true;
	else
		return false;
};

int runCMD(const char* cmd) {
	printf("run %s\n", cmd);
	int err = system(cmd);
	printf("return %d\n", err);
	return err;
}
int runCMD(const string& cmd) {
	return runCMD(cmd.c_str());
}

void displayHelp(const char* cmd0) {
	printf("My Live Tool\n");
	printf("See https://github.com/KZ25T/My-Live-Tools \n");
	printf("Usage:\n");
	// set progs
	printf(
		"%s --help(or -h):                "	 // cmds
		"display help info\n",
		cmd0);
	printf(
		"%s --config-path(or -c) PATH:    "	 // cmds
		"specify files path(.live/%s)\n",
		cmd0, PLATFORM);
	printf(
		"%s --mount-dev(or -m) ROOTPATH:  "	 // cmds
		"mount  /proc, /sys, /dev, /run, /dev/shm on ROOTPATH\n",
		cmd0);
	printf(
		"%s --umount-dev(or -u) ROOTPATH: "	 // cmds
		"umount /proc, /sys, /dev, /run, /dev/shm on ROOTPATH\n",
		cmd0);

	return;
}

int main(int argc, const char* argv[]) {
	if (argc == 1) {
		printf("%s error: args missing.\n", argv[0]);
		displayHelp(argv[0]);
		return 1;
	}
	if (argc == 2) {
		if (streql(argv[1], "--startup") || streql(argv[1], "-s")) {
			// run while starting up
			pid_t pid	 = fork();
			int	  status = 0;
			if (pid == 0) {
				// I don't know why we need a new PID to run syscall
				LoadConfig ld;
				if (ld.success()) {
					printf("detected config file!\n");
					DIR* dir = ld.getDir();
					runProg(dir, cfgPoint, true);
				}
				return 0;
			}
			else {
				while (waitpid(pid, &status, 0) < 0) {
					if (errno != EINTR) {
						status = -1;
						break;
					}
				}
			}
			//                          _ooOoo_                               //
			//                         o8888888o                              //
			//                         88" . "88                              //
			//                         (| ^_^ |)                              //
			//                         O\  =  /O                              //
			//                      ____/`---'\____                           //
			//                    .'  \\|     |//  `.                         //
			//                   /  \\|||  :  |||//  \                        //
			//                  /  _||||| -:- |||||-  \                       //
			//                  |   | \\\  -  /// |   |                       //
			//                  | \_|  ''\---/''  |   |                       //
			//                  \  .-\__  `-`  ___/-. /                       //
			//                ___`. .'  /--.--\  `. . ___                     //
			//              ."" '<  `.___\_<|>_/___.'  >'"".                  //
			//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
			//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
			//      ========`-.____`-.___\_____/___.-`____.-'========         //
			//                           `=---='                              //
			//      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^        //
			//  Buddha blesses us to never shut down and never have any bugs  //
			if (status != 0) {
				printf("file detect and install failed in %d!", status);
				return status;
			}
			umount(mntPoint);
			rmdir(mntPoint);
			return 0;
		}
		if (streql(argv[1], "--help") || streql(argv[1], "-h")) {
			displayHelp(argv[0]);
			return 0;
		}
		// not supported command
		printf("%s error: not supported options: %s.\n", argv[0], argv[1]);
		displayHelp(argv[0]);
		return 1;
	}
	if (argc == 3) {
		string argv2(argv[2]);
		if (*argv2.rbegin() == '/') argv2.erase(argv2.end() - 1);
		if (streql(argv[1], "--config-path") || streql(argv[1], "-c")) {
			runProg(nullptr, argv2.c_str(), false);
			return 0;
		}
		if (streql(argv[1], "--mount-dev") || streql(argv[1], "-m")) {
			printf("see https://wiki.archlinux.org/title/Chroot#Using_chroot \n");
			runCMD(format("mount -t proc  /proc {}/proc/", argv2));
			runCMD(format("mount -t sysfs /sys  {}/sys/", argv2));
			runCMD(format("mount --bind /sys/firmware/efi/efivars {}/sys/firmware/efi/efivars", argv2));
			runCMD(format("mount --rbind  /dev  {}/dev/", argv2));
			runCMD(format("mount --rbind  /run  {}/run/", argv2));
			runCMD(format("mount -t tmpfs  shm  {}/dev/shm/", argv2));
			return 0;
		}
		if (streql(argv[1], "--umount-dev") || streql(argv[1], "-u")) {
			printf("see https://wiki.archlinux.org/title/Chroot#Using_chroot \n");
			runCMD(format("umount {}/dev/shm", argv2));
			runCMD(format("mount --make-rslave {}/run/", argv2));
			runCMD(format("umount -R {}/run/", argv2));
			runCMD(format("mount --make-rslave {}/dev/", argv2));
			runCMD(format("umount -R {}/dev/", argv2));
			runCMD(format("umount {}/sys/firmware/efi/efivars", argv2));
			runCMD(format("umount {}/sys/", argv2));
			runCMD(format("umount {}/proc", argv2));
			return 0;
		}
		// not supported command
		printf("%s error: not supported options: %s %s\n", argv[0], argv[1], argv[2]);
		displayHelp(argv[0]);
		return 1;
	}
	printf("%s error: args invalid.\n", argv[0]);
	displayHelp(argv[0]);
	return 1;
}
