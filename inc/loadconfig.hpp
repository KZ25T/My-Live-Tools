#pragma once
#include "main.hpp"
#include <blkid/blkid.h>
#include <dirent.h>
#include <string>
#include <sys/mount.h>
#include <vector>

using std::string;
using std::vector;
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
		enum fstype { VFAT, EXFAT, EXT4, XFS, BTRFS, ISO9660, NTFS, OTHER };
		string deviceName;
		string deviceLabel;
		fstype deviceFSType;
		DeviceItem(blkid_dev device);
		const char* GetFSStr();
		void		print(std::ostream& os);
		bool		operator<(const DeviceItem& another);
	};
	class GetDevices {
	public:
		GetDevices();
		bool GetMountedList();
		DIR* GetConfigFile();

	private:
		vector<DeviceItem> deviceList;
		string			   USBDeviceName;
	};
};
