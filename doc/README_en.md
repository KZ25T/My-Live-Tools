# Instructions for use

中文(Chinese):[GitHub](https://github.com/KZ25T/My-Live-Tools/blob/master/doc/README.md)或[Gitee](https://gitee.com/KZ25T/my-live-tools/blob/master/doc/README.md) Note: this document is translated by translation website from the Chinese version, some sentences may not be checked and maybe not correct.

This livecd is an open-source version created by Linux enthusiasts, mainly aimed at Chinese users, and can perform tasks such as disk management, network connectivity, and software development.

**Important update** This system supports small-scale customization based on the original image, please refer to [LivecdTool](#5livecd-tool)

[TOC]

## 1.User password

No passwords for users `root` and `user` and `sudo` command. Please add password by yourself if needed.

## 2.Basic information

Basic system information:

- Architecture: amd64, x86_64
- Release: Debian 12.5
- Kernel version: 6.1.0
- Graphics desktop: xfce4
- Login manager: lightdm (configured for automatic login)

## 3.Pre-installed commonly used software

For convenience and practicality, I have pre installed some commonly used software here, including:

- Browser: Edge Browser
  - Launch method: taskbar, or command: `microsoft-edge-stable`
- Input Method: Sogou Pinyin Input Method
  - Based on the input method of fcitx, press `shift` to switch between Chinese and English in the tray.
- There is an issue with the Qt library that comes with this version of Sogou. The author used the system Qt library to fix it.
- Communication software: QQ(One of the most popular communication software in China)
  - Startup method: taskbar, or command: `/opt/QQ/QQ`
  - WeChat has not yet provided a reliable Linux version, so it has not been installed. If needed, you can download [Ukylin WeChat](https://www.ubuntukylin.com/applications/106-cn.html)Installation and use.
- Internet tool: NetworkManager
  - Click on the tray network icon to access the internet, or use the command `nm-applet` to start.
- Development tool: VSCode
  - Startup method: taskbar, or command: `code` or `code somepath`
- Development tool: vim
  - According to the author's preferred VIM configuration, it has been preset.
- Disk management: gparted
  - Several common file system tools have been installed.
- Hardware management: hardinfo
- Network packet capture software: Wireshark
- Compilation chain tool: build essential, which can use commands such as gcc.

Other commonly used commands:

- Adjust the terminal language using either `zh` or `en`. For the hardinfo, gparted and other applications mentioned above, they are opened in English by default from the terminal. You can enter zh to adjust to Chinese and then open them. You can also set environment variables in the front, such as `LANG=zh_cN.UTF-8 hardinfo`
- Set desktop and other displays: `xfce4 settings manager`
- Command line internet access (alternative to internet access): `nmtui`
- View command usage: `tldr`, for example: `tldr tar`
Partial shortcut keys:
- Display desktop: `win (super)+D`
- Open Terminal: `win (super)+R`
- Open File Manager: `win (super)+E`

## 4.Other changes

Debian is a very clean system, and in order to make it useful, I have introduced some closed source software or other content. The following are some changes (excluding the previous pre installed software):

- Modify the APT source to [USTC Source](https://mirrors.ustc.edu.cn/help/) in China (Thanks to the USTC for its open source software image support.)
- Install some commonly used open source software.
- Installed proprietary firmware for some Intel CPUs and NVIDIA graphics cards.
- Modified the user's desktop and added three images, all taken by the author themselves. The first two photos were taken at the Liangzhu Ancient City Ruins Park, while the third one was taken at Baima Lake in Hangzhou, which is assumed to be the first one. Commercial use is prohibited.
- Change the system theme to a theme similar to Windows 10.
- Modify the user terminal to zsh and configure it according to the author's preferred method.
- Modify commonly used editors and text browsers to Vim and configure them.
- Pre installed Chinese environment, but the system is still in English.
- Add fonts console and msyh as equal width fonts and commonly used fonts, respectively.
This system is a free tool designed for enthusiasts and is not intended for any commercial use. I have not subjectively introduced any malicious programs or computer viruses. If some of the installed software comes with viruses, please find the corresponding software provider to resolve the issue. When using this operating system, please comply with the laws and regulations of your country or region. The author is not responsible for any consequences caused by using this system.

## 5.Livecd tool

### 5.1 Startup Functions

This system supports the following functions:

- Add files on existing systems.
  Function: For example, to create a file called `/home/user/Desktop/picture. png` and `/usr/bin/yourcmd` after startup.
- Install the deb package at startup.
- Run the script at startup.

### 5.2 Startup Brief usage method

1. On the USB drive with the system image installed, select any partition to load the resource files of this tool. If you are mounting the image using Ventoy, you shouled not place it in Ventoy(where place the iso files) or VOTYEFI partition.(Tips: you can reserve some place when install Ventoy by adjusting options, and this place can be used to format as a partitation to place the resource file required by this tool.)
   Requirement: The file system format for this partition is one of vfat (fat32), exfat, ext4, xfs, btrfs, iso9660, or ntfs(usually exfat for USB drives). We run `ntfs-3g` when mounting ntfs, other filesystem type are mounted by system call.
2. Create a directory in this partition called `.live` with an absolute path of `/Some/mount/point/.live/OSNAME`(OSNAME is like `debian` or `kali` and so on)
3. If you need to use the first function:
   - Find a working directory, such as workfolder
   - Create Root Directory: `mkdir rootfs`
   - For example, if you want to have `/home/user/Desktop/picture.png` and `/usr/bin/yourcmd` after startup, then move them to rootfs to make

     ```bash
     $ tree rootfs/ 
     rootfs/
     ├── home
     │   └── user
     │       └── Desktop
     │           └── picture.png
     └── usr
         └── bin
             └── yourcmd
     6 directories, 2 files
     ```

   - Packaging: `cd rootfs && zip /home/mount/point/OSNAME/.live/overlay.zip -r .`
   - Encrypted zip packages or other advanced settings are not currently supported.
   - Try not to generate this compressed file on Windows. If the compressed file is generated using Windows, please ensure that when opened by double clicking, it becomes the root directory (visible as home or usr, etc.), and do not use Chinese paths or file names.
   - Tip: If you are a Ventoy user, then Ventoy has similar functions (but the implementation principle is different from mine), please refer to [relevant instructions](https://www.ventoy.net/en/doc_live_injection.html). (My functionality has not been fully tested, Ventoy's testing is definitely stronger than mine)
   - Tip: Possible uses of this feature:
     - Add frequently used files to desktop: `/home/user/Desktop/common files`
     - Overwrite default zsh configuration: `/home/user/. zshrc`
     - Provide git configuration so that after booting up, the git will follow your configuration: `/home/user/.gitconfig` and `.git-credentials`
     - Provide SSH/GPG key configuration: `/home/user/.ssh/` or `/home/user/.gnupg/` to enable direct SSH after startup. **Safety reminder: If installing this configuration, please be careful to prevent related file leaks.**
     - Provide WiFi configuration: `/etc/NetworkManager/system connections/WIFI_NAME.nmconnection` can automatically connect to WiFi after startup.
   - Tip: If you want to replace an existing directory on this livecd with your own (for example, replacing the existing `/home/user/.vim` with your own), please first write a command in `preScript` (see below) to delete the original directory.
4. If you need to use the second function:
   - Create directory: `mkdir /home/mount/point/.live/OSNAME/packages`
   - Add the deb package that needs to be installed to the above directory.
   - I am calling dpkg installation and cannot use APT to handle dependencies. Please remember to download the complete dependencies.
   - Tips: `apt dependencies xxx` View dependencies (please recursively query), `apt download xxx` Download the deb package from the software source. Please pay attention to dependency relationships and dependent versions, so it is suggested to use packages without dependencies, such as WPS office(a popular doc/xls/ppt editor in China).
5. If you need to use the third function:
   - If you want to run ahend the other two functions, please place the script at `/home/mount/point/.live/OSNAME/preScript`
   - If you want to run after the other two functions, please place the script at `/home/mount/point/.live/OSNAME/postScript`

### 5.3 Other functions

1. `mlt --config-path(or -c) PATH` siecify configure file path, when`.live/OSname` does not exist, or in special conditions like USB drive is not `/dev/sdX`, you can use this option to complete. The three files/folder should under PATH.
2. `mlt --mount-dev(or -m) PATH` mounts dev, proc, sys to chroot path. The same as:

   ```bash
   mount -t proc  /proc ${PATH}/proc/
   mount -t sysfs /sys  ${PATH}/sys/
   mount --bind /sys/firmware/efi/efivars ${PATH}/sys/firmware/efi/efivars/
   mount --rbind  /dev  ${PATH}/dev/
   mount --rbind  /run  ${PATH}/run/
   mount -t tmpfs  shm  ${PATH}/dev/shm/
   ```

3. `mlt --umount-dev(或 -u) PATH` unmount what mounted above. The same as:

   ```bash
   umount ${PATH}/dev/shm
   mount --make-rslave ${PATH}/run/
   umount -R ${PATH}/run/
   mount --make-rslave ${PATH}/dev/
   umount -R ${PATH}/dev/
   umount ${PATH}/sys/firmware/efi/efivars
   umount ${PATH}/sys
   umount ${PATH}/proc
   ```

### 5.4 Precautions

1. This program starting-up functions only searches for the partition of the USB drive that contains this Livecd image. The search order is: first, search for the partition labeled Ventoy, then search for other partition of the USB drive(not labeled Ventoy or VOTYEFI) in linux label number order until a partition containing the `.live/OSNAME` directory is found (and meets the file system requirements mentioned above).
2. When running this program on starting up, the permission is root, and all mounted partitions are read-only. I have only tested the relevant content on directories and files, and I am not sure if there will be any adverse side effects on files such as links. All files under `.live/OSNAME` must be regular file or direction, can not be links.
3. When using the first starting-up function, it is not possible to overwrite a directory with a file in a certain location, or to overwrite a file with a directory.
4. The performed order is: `preScript` -> `Function1` -> `Function2` -> `postScript`, and only run when the required file can be detected.
5. The location of this tool is `/usr/local/bin/mlt`, which is a static compiled program.
6. While starting up and running this program, the loaction of `.live` is `/tmp/mountpoint/.live`(you can refer while writing scripts if needed.)

### 5.5 Tool source code

[GitHub](https://github.com/KZ25T/My-Live-Tools) or [Gitee](https://gitee.com/KZ25T/my-live-tools)

## 6. Installing the system

If you really like my system, you can try installing it. You need to have some Linux knowledge to install it, otherwise it is recommended to still follow the [official website](https://www.debian.org/distrib/)And refer to the author's notes on [precautions](https://blog.csdn.net/m0_57309959/article/details/135856767)Installation.

**Reminder:** This function is not yet complete and has just been tested by myself simply(It should be no problem). If damage is caused by improper operation, please bear the responsibility yourself.

For example, installing debian. Steps:

- Start this operating system.
- Start GParted(command: `sudo gparted`) to partition the hard disk, allocate hard disk space for installing this system(at least an ext4 filesystem for root partitation), and mount it on `/mnt/debian`. (Of course, the mounting point is arbitrary)
- Tip: If your hard drive already has Windows (Windows 10 or 11) or Linux installed, there should be an ESP partition on the hard drive (opened with gparted, labeled as boot, esp, etc., formatted as fat32); If not(such a new or empty hard disk), then you need to manually create a few hundred MB partition with the format fat32 as the ESP partition; Then mount your ESP partition to `/mnt/debian/boot/efi`.
  - Example: If your partition is adjusted to:
    ![1](1.png)
    Then:
    - Mount root directory: `sudo mkdir -p /mnt/debian && sudo mount /dev/sda2 /mnt/debian`
    - Mount ESP partition: `sudo mkdir -p /mnt/debian/boot/efi && sudo mount /dev/sda2 /mnt/debian/boot/efi`
- Copy content outside the entire system startup section: `sudo cp -rpv /run/live/rootfs/filesystem.squashfs/* /mnt/debian`
- Installing the kernel
  - copy kernel and initramfs file: `sudo cp -pv /run/live/medium/live/vmlinuz /run/live/medium/live/initrd.img /mnt/debian/boot`
- Install grub and configure boot
  - If your hard drive already has a grub (which is common when your computer already has a Linux operating system and will enter the grub page upon startup), then you don't need to install grub, just press back to the original Linux system and execute `sudo update grub` (some distributions use `grub-mkconfig -o /boot/grub/grub.cfg`) (remember to enable os probe)
  - If your hard drive does not have Grub (which is common on your computer and may only have one Windows 10/11), you need to install Grub:
    - Complete chroot: `sudo mlt -m /mnt/debian`, then `sudo chroot /mnt/debian /bin/bash`
    - Install grub: `(chroot) grub-install --boot-directory=/boot --efi-directory=/boot/efi YOUR_HARD_DRIV_PATH(e.g. /dev/sda)`
    - Configure system probe: `(chroot) vim /etc/default/grub`, Set the line of `GRUB_DISABLE_OS_PROBER` to false (usually in the commented state, please cancel the previous comment)
    - Configure grub file: `(chroot) update-grub`, then ` (chroot) vim /boot/grub/grub.cfg `, and add the following content after the line of `BEGIN /etc/grub.d/30_os-prober`:

      ```text
      menuentry 'Debian' {
          insmod part_gpt
          insmod ext2
          search --no-floppy --fs-uuid --set=root root_directory_hard_disk_partition_UUID
          linux /boot/vmlinuz root=UUID=root_directory_hard_disk_partition_UUID
          initrd /boot/initrd.img
      }
      ```

    The `root_directory_hard_disk_partition_UUID` is obtained by using the `blkid` command to find the corresponding UUID entry for the root directory partition.
- Exit the chroot: ctrl+D or exit command, then `sudo mlt -u /mnt/debian`
- Set up mounting system:
  - List the UUIDs of each partition through blkid, mainly focus on root directory partitions and ESP partitions.
  - Set to `/etc/fstab` in the new os, for example:

    ```text
    sudo touch /mnt/debian/etc/fstab
    sudoedit /mnt/debian/etc/fstab
    ```

    add the following content:

    ```text
    UUID=xxxxxxxx-xxxx-xxxx-xxxx-xxxx /          ext4    errors=remount-ro 0 1
    UUID=xxxx-yyyy                    /boot/efi  vfat    umask=0077        0 1
    ```

- Unmount os: `sudo umount /mnt/debian/boot/efi && sudo umount /mnt/debian`
- After restarting, install necessary software, reset passwords, cancel automatic login, update apt source and software, etc.

## 7. Contact the author

Email:`ikun@mail.ustc.edu.cn`

The partial production methods of this system have been made public by the author on the CSDN website: [Part of the content](https://blog.csdn.net/m0_57309959)(write in Chinese)

If you have the author's contact information such as QQ or WeChat, you can also contact directly.
