#pragma once
#include <blkid/blkid.h>
#include <dirent.h>
#include <string>
#include <sys/mount.h>
#include <vector>
const char* const mountPoint  = "/tmp/mountpoint";
const char* const configPoint = "/tmp/mountpoint/.live";

class LoadConfig {
public:
	LoadConfig();
	~LoadConfig();
	bool success();
	DIR* getDir();

private:
	DIR* cfgDir;
	struct DeviceItem
	{
		enum fstype { VFAT, EXFAT, EXT4, XFS, BTRFS, ISO9660, OTHER };
		std::string deviceName;
		std::string deviceLabel;
		fstype		deviceFSType;
		DeviceItem(blkid_dev device);
		const char* GetFSStr();
		void		print(std::ostream& os);
		bool		operator<(const DeviceItem& another);
	};
	class GetDevices {
	public:
		GetDevices();
		GetDevices(const char* devicePath);
		bool GetMountedList();
		DIR* GetConfigFile();
	private:
		std::vector<DeviceItem> deviceList;
		std::string				USBDeviceName;
	};
};
