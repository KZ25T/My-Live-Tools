#include "loadconfig.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
/**
 * @brief create a new device item
 *
 * @param device device ptr
 */
LoadConfig::DeviceItem::DeviceItem(blkid_dev device) {
	deviceName			   = string(blkid_dev_devname(device));
	deviceLabel			   = "";
	deviceFSType		   = OTHER;
	blkid_tag_iterate iter = blkid_tag_iterate_begin(device);
	const char *	  type, *value;
	while (blkid_tag_next(iter, &type, &value) == 0) {
		if (strcmp(type, "LABEL") == 0) {
			deviceLabel = string(value);
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
/**
 * @brief Convert deviceFSType to str
 *
 * @return const char* fstype string
 */
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
/**
 * @brief load for debug
 *
 * @param os
 */
void LoadConfig::DeviceItem::print(std::ostream& os) {
	os << deviceName << ", label=" << deviceLabel << ", type=" << GetFSStr() << std::endl;
}
/**
 * @brief for std::sort
 *
 * @param another
 * @return true
 * @return false
 */
bool LoadConfig::DeviceItem::operator<(const DeviceItem& another) {
	int lval = atoi(deviceName.c_str() + 8);  // 8: /dev/sdx
	int rval = atoi(another.deviceName.c_str() + 8);
	if (lval < rval)
		return true;
	else
		return false;
}
/**
 * @brief Get Devices, while get mounted devices and usb device name, then search all
 * block devices under the same usb name.
 */
LoadConfig::GetDevices::GetDevices() {
	USBDeviceName = "";
	// Get Mounted devices
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
		// should be the same usb device
		if (beginWith(blkid_dev_devname(dev), USBDeviceName.c_str()) == true)
			deviceList.push_back({dev});
	}
	std::sort(deviceList.begin(), deviceList.end());
	blkid_dev_iterate_end(iter);
	blkid_put_cache(cache);
	return;
}
/**
 * @brief get mounted devices and usb device name
 *
 * @return true get success
 * @return false not success
 */
bool LoadConfig::GetDevices::GetMountedList() {
	std::ifstream mounts("/proc/self/mounts", std::ios_base::in);
	string	  mountedBlock;
	if (mounts.bad()) return false;
	while (true) {
		char buf[160];
		mounts.getline(buf, sizeof(buf));
		if (mounts.eof()) break;
		// usb should as /dev/sdxN
		if (beginWith(buf, "/dev/sd") == true) {
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
			mountedBlock = string(buf);
		}
	}
	if (mountedBlock.size() == 0) return false;	 // should be only one
	USBDeviceName = mountedBlock.substr(0, 8);
	return true;
}
/**
 * @brief get config file DIR ptr
 *
 * @return DIR* config file DIR, should be /tmp/mountpoint/.live
 */
DIR* LoadConfig::GetDevices::GetConfigFile() {
	// create mount point
	if (deviceList.size() == 0) return nullptr;
	DIR* dp;
	if ((dp = opendir(mntPoint)) == nullptr)
		// mountpoint does not exist
		mkdir(mntPoint, S_IRWXU);
	else {
		if (readdir(dp) != nullptr) {
			// mountpoint is not empty
			std::cout << "error: folder exists" << std::endl;
			closedir(dp);
			return nullptr;
		}
		// mountpoint is empty
	}
	// get device name, first search Ventoy(should be only one Ventoy in one USB)
	for (auto item : deviceList) {
		if (item.deviceLabel == "Ventoy" &&
			item.deviceFSType != DeviceItem::fstype::OTHER) {
			// mount ventoy
			int mounterr = mount(
				item.deviceName.c_str(), mntPoint, item.GetFSStr(), MS_RDONLY, nullptr);
			if (mounterr != 0) break;
			if ((dp = opendir(cfgPoint)) != nullptr) {
				// exist!
				return dp;
			}
			else {
				// do not exist, umount, try other all fs.
				umount(mntPoint);
				break;
			}
		}
	}
	// get other device name
	for (auto item : deviceList) {
		if (item.deviceLabel != "Ventoy" && item.deviceLabel != "VOTEFI" &&
			item.deviceFSType != DeviceItem::fstype::OTHER) {
			// mount fs
			int mounterr = mount(
				item.deviceName.c_str(), mntPoint, item.GetFSStr(), MS_RDONLY, nullptr);
			if (mounterr != 0) continue;
			if ((dp = opendir(cfgPoint)) != nullptr) {
				// exist!
				return dp;
			}
			else {
				// do not exist, umount, try other all fs.
				umount(mntPoint);
				continue;
			}
		}
	}
	rmdir(mntPoint);
	return nullptr;
}

LoadConfig::LoadConfig() {
	cfgDir = GetDevices().GetConfigFile();
}
LoadConfig::~LoadConfig() {
	if (cfgDir != nullptr) { rmdir(mntPoint); }
}
bool LoadConfig::success() {
	if (cfgDir != nullptr) return true;
	return false;
}
DIR* LoadConfig::getDir() {
	return cfgDir;
}
