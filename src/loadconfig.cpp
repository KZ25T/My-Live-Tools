#include "loadconfig.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
static bool BeginWith(const char* str, const char* header) {
	while (*header != 0) {
		if (*str != *header) return false;
		str++;
		header++;
	}
	return true;
}
LoadConfig::DeviceItem::DeviceItem(blkid_dev device) {
	deviceName			   = std::string(blkid_dev_devname(device));
	deviceLabel			   = "";
	deviceFSType		   = OTHER;
	blkid_tag_iterate iter = blkid_tag_iterate_begin(device);
	const char *	  type, *value;
	while (blkid_tag_next(iter, &type, &value) == 0) {
		if (strcmp(type, "LABEL") == 0) {
			deviceLabel = std::string(value);
			continue;
		}
		if (strcmp(type, "TYPE") == 0) {
			if (strcmp(value, "vfat") == 0)
				deviceFSType = VFAT;
			else if (strcmp(value, "exfat") == 0)
				deviceFSType = EXFAT;
			else if (strcmp(value, "ext4") == 0)
				deviceFSType = EXT4;
			else if (strcmp(value, "xfs") == 0)
				deviceFSType = XFS;
			else if (strcmp(value, "btrfs") == 0)
				deviceFSType = BTRFS;
			else if (strcmp(value, "iso9660") == 0)
				deviceFSType = ISO9660;
			else
				deviceFSType = OTHER;
		}
	}
	blkid_tag_iterate_end(iter);
}
const char* LoadConfig::DeviceItem::GetFSStr() {
	switch (deviceFSType) {
		case VFAT: return "vfat";
		case EXFAT: return "exfat";
		case EXT4: return "ext4";
		case XFS: return "xfs";
		case BTRFS: return "btrfs";
		case ISO9660: return "iso9660";
		default: return "other";
	}
}
void LoadConfig::DeviceItem::print(std::ostream& os) {
	os << deviceName << ", label=" << deviceLabel << ", type=" << GetFSStr() << std::endl;
}

LoadConfig::GetDevices::GetDevices() {
	// Get Mounted devices
	mountedBlock  = "";
	USBDeviceName = "";
	if (GetMountedList() == false) return;
	blkid_cache cache = nullptr;
	if (blkid_get_cache(&cache, nullptr) < 0) return;
	blkid_probe_all(cache);
	blkid_dev		  dev;
	blkid_dev_iterate iter = blkid_dev_iterate_begin(cache);
	blkid_dev_set_search(iter, nullptr, nullptr);
	while (blkid_dev_next(iter, &dev) == 0) {
		dev = blkid_verify(cache, dev);
		if (!dev) continue;
		if (BeginWith(blkid_dev_devname(dev), USBDeviceName.c_str()) == true)
			deviceList.push_back(DeviceItem(dev));
	}
	blkid_dev_iterate_end(iter);
	blkid_put_cache(cache);
	return;
}
LoadConfig::GetDevices::GetDevices(const char* devicePath) {
	blkid_cache cache = nullptr;
	if (blkid_get_cache(&cache, nullptr) < 0) return;
	blkid_dev dev = blkid_get_dev(cache, devicePath, BLKID_DEV_NORMAL);
	if (!dev) return;
	deviceList.push_back(DeviceItem(dev));
	blkid_put_cache(cache);
}
// Get Mounted List and Get USB Device Name
bool LoadConfig::GetDevices::GetMountedList() {
	std::ifstream mounts("/proc/self/mounts", std::ios_base::in);
	if (mounts.bad()) return false;
	while (true) {
		char buf[160];
		mounts.getline(buf, sizeof(buf));
		if (mounts.eof()) break;
		// usb should as /dev/sdxN
		if (BeginWith(buf, "/dev/sd") == true) {
			char* p = buf;
			while (*p != ' ') p++;
			*p = 0;
			// have collected mounted block
			if (mountedBlock.size() != 0) {
				if (strcmp(buf, mountedBlock.c_str()) != 0)
					// this is not what have collected
					// should not happen
					return false;
				else
					// another collected device
					continue;
			}
			mountedBlock = std::string(buf);
		}
	}
	if (mountedBlock.size() == 0) return false;	 // should be only one
	USBDeviceName = mountedBlock.substr(0, 8);
	return true;
}
// BUG:ERROR WHEN HAPPEN IN BLOCKS HAVE MOUNTED
DIR* LoadConfig::GetDevices::GetConfigFile() {
	// create mount point
	if (deviceList.size() == 0) return nullptr;
	for (auto item : deviceList) { item.print(std::cout); }
	DIR* dp;
	if ((dp = opendir(mountPoint)) == nullptr)
		mkdir(mountPoint, S_IRWXU);
	else {
		std::cerr << "error: folder exists" << std::endl;
		closedir(dp);
		return nullptr;
	}
	// get device name
	for (auto item : deviceList) {
		if (item.deviceLabel == "Ventoy" &&
			item.deviceFSType != DeviceItem::fstype::OTHER) {
			// mount ventoy
			int mounterr = mount(
				item.deviceName.c_str(), mountPoint, item.GetFSStr(), MS_RDONLY, nullptr);
			if (mounterr != 0) break;
			if ((dp = opendir(configPoint)) != nullptr) {
				// exist!
				return dp;
			}
			else {
				// do not exist, umount, try other all fs.
				umount(mountPoint);
				break;
			}
		}
	}
	for (auto item : deviceList) {
		if (item.deviceFSType != DeviceItem::fstype::OTHER &&
			item.deviceName != mountedBlock) {
			// mount fs
			int mounterr = mount(
				item.deviceName.c_str(), mountPoint, item.GetFSStr(), MS_RDONLY, nullptr);
			if (mounterr != 0) continue;
			if ((dp = opendir(configPoint)) != nullptr) {
				// exist!
				return dp;
			}
			else {
				// do not exist, umount, try other all fs.
				umount(mountPoint);
				continue;
			}
		}
	}
	rmdir(mountPoint);
	return nullptr;
}

void LoadConfig::GetDevices::debug() {
	for (auto item : deviceList) { item.print(std::cout); }
}

LoadConfig::LoadConfig() {
	cfgDir = GetDevices().GetConfigFile();
}
LoadConfig::~LoadConfig() {
	if (cfgDir != nullptr) { rmdir(mountPoint); }
}
bool LoadConfig::success() {
	if (cfgDir != nullptr) return true;
	return false;
}