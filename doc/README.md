# 使用说明

English Version:[GitHub](https://github.com/KZ25T/My-Live-Tools/blob/master/doc/README_en.md) or [Gitee](https://gitee.com/KZ25T/my-live-tools/blob/master/doc/README_en.md)

这个 livecd 是一个 Linux 爱好者自行制作的开源版，主要面向中国用户，可以进行磁盘管理、网络连接、软件开发等工作。

**重要更新** 本系统支持在原有镜像基础上进行小规模定制，参考[Livecd工具](#5livecd工具)

[TOC]

## 1.用户密码

用户 `root` 和 `user` 的密码都是 `passwd`

## 2.基本信息

系统基本信息：

- 架构：amd64, x86_64
- 发行版：Debian 12.4
- 内核版本：6.1.0
- 图形桌面：xfce4
- 登录管理器：lightdm（已配置为自动登录）

## 3.预装常用软件

为了方便实用，我这里预装了部分常用软件，包括：

- 浏览器：edge 浏览器
  - 启动方式：任务栏，或命令：`microsoft-edge-stable`
- 输入法：搜狗拼音输入法
  - 基于 fcitx 的输入法，在托盘内，按 `shift` 切换中英文。
  - 这个版本搜狗自带的 Qt 库有问题，作者使用系统 Qt 库修复之。
- 通讯软件：QQ
  - 启动方式：任务栏，或命令：`/opt/QQ/qq`
  - 微信暂未给出可靠的 Linux 版本，所以没有安装，如有需要可以下载[优麒麟微信](https://www.ubuntukylin.com/applications/106-cn.html)安装使用。
- 上网工具：NetworkManager
  - 托盘网络图标点击上网，或用命令 `nm-applet` 启动。
- 开发工具：VSCode
  - 启动方式：任务栏，或命令：`code` 或 `code somepath`
- 开发工具：vim
  - 按照作者本人喜欢的 vim 配置进行了预设。
- 磁盘管理：gparted
  - 已安装常见的若干文件系统工具。
- 硬件管理：hardinfo
- 抓包软件：wireshark
- 编译链工具：build-essential，可以使用 gcc 等命令。

其他常用命令：

- `zh` 或 `en` 调整终端语言。对于上面的 hardinfo，gparted 等应用，默认从终端打开为英文，可以输入 zh 调整为中文后打开，也可以在前边设置环境变量，如：`LANG=zh_CN.UTF-8 hardinfo`
- 设置桌面等显示：`xfce4-settings-manager`
- 命令行上网（上网备选方案）：`nmtui`
- 查看命令用法：`tldr`，如：`tldr tar`

部分快捷键：

- 显示桌面：`win(super)+D`
- 打开终端：`win(super)+R`
- 打开文件管理器：`win(super)+E`

## 4.其他改动

Debian 是一个很干净的系统，为了使得其变得好用，我引入了一些闭源软件或其他内容。以下是部分改动（不包括上一条预装软件）：

- 修改 apt 源为[中科大源](https://mirrors.ustc.edu.cn/help/)（感谢中国科学技术大学开源软件镜像支持）。
- 安装一些常用开源软件。
- 安装了部分 intel cpu 和 nvidia 显卡的专有固件。
- 修改用户桌面，添加了三张图，均为作者自己拍摄。前两张的拍摄地点为良渚古城遗址公园，第三张是杭州白马湖，默认为第一张。禁止商用。
- 修改系统主题为类似 Windows 10 的主题。
- 修改用户终端为 zsh 并按照作者本人喜爱的方式进行配置。
- 修改常用编辑器、文字浏览器为 vim 并进行配置。
- 预装中文环境，但系统仍为英文。
- 添加字体 consolas 和 msyh，分别作为等宽字体和常用字体。

本系统为爱好者制作的免费工具，不做任何商业用途。本人未主观引入恶意程序或计算机病毒，如果是安装的某些软件自带病毒，那么请找到对应的软件提供者解决。使用此操作系统时，请遵守所在国家或地区的法律规定，使用本系统造成的一切后果，作者不承担责任。

## 5.Livecd工具

### 5.1功能

本系统支持以下功能：

- 在已有系统上添加文件。
  作用：如使得启动后有一个文件 `/home/user/Desktop/picture.png` 和 `/usr/bin/yourcmd`
- 在启动时安装 deb 包。
- 在启动时运行脚本。

### 5.2简要使用方法

1. 在装有本系统镜像的 U 盘上，任选一个分区装载本工具的资源文件。如果您是用 Ventoy 装载的镜像，那么建议放在 Ventoy 分区内（也就是 iso 文件所在的位置），不要放在 VOTEFI 分区内。
   要求：此分区的文件系统格式为 vfat(fat32), exfat, ext4, xfs, btrfs, iso9660 中的一个（一般 U 盘是 exfat），不能是 ntfs 格式。
2. 在该分区中创建一个目录 `.live`，其绝对路径为 `/some/mount/point/.live`
3. 若需要使用第一个功能：
   - 找一个工作目录，如 workfolder
   - 创建根目录：`mkdir rootfs`
   - 比如说你希望启动后有 `/home/user/Desktop/picture.png` 和 `/usr/bin/yourcmd`，那么将其移动到 rootfs 下，使得

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

   - 打包：`cd rootfs && zip /some/mount/point/.live/overlay.zip -r .`，即产生 `overlay.zip` 放在 .live 下。
   - 暂不支持加密的 zip 包，或其他的高级设置。
   - 尽量不要在 Windows 上产生该压缩包。如果该压缩包用 Windows 产生，请保证双击打开后即为根目录（可见 home 或 usr 等），且不要使用中文路径或文件名。
   - 提示：如果你是 Ventoy 用户，那么 Ventoy 有类似的功能（但实现原理和我的不一样），参考[相关说明](https://www.ventoy.net/cn/doc_live_injection.html)。（我的功能未经过完备测试，Ventoy 的测试肯定比我强）
   - 提示：本功能的可能用处：
     - 添加常用文件到桌面：`/home/user/Desktop/common-files`
     - 覆盖默认的 zsh 配置：`/home/user/.zshrc`
     - 提供 git 配置，使得开机后的 git 就按照已有的配置：`/home/user/.gitconfig` 和 `.git-credentials`
     - 提供 ssh/gpg 密钥配置：`/home/user/.ssh/` 或 `/home/user/.gnupg/` 使得开机后就能直接 ssh 等。**安全提示：如安装此配置，请谨防相关文件泄漏。**
     - 提供 wifi 配置：`/etc/NetworkManager/system-connections/WIFI名字.nmconnection` 可以开机之后自动连接 wifi
4. 若需使用第二个功能：
   - 创建目录：`mkdir /some/mount/point/.live/packages`
   - 将需要安装的 deb 包添加至以上目录内。
   - 本人调用 dpkg 安装，不能使用 apt 处理依赖，请记得下载完整依赖。
   - 小技巧：`apt depends xxx` 查看依赖（请递归查询），`apt download xxx` 下载软件源里的 deb 包。请注意依赖关系及依赖版本，所以最好使用没有依赖的包。
5. 若需使用第三个功能：
   - 将脚本复制为 `/some/mount/point/.live/startup-scripts`

### 5.3注意事项

1. 本程序只查找装有此 Livecd 镜像的 U 盘的分区，查找顺序为：首先查找标签为 Ventoy 的分区，然后按照编号数字顺序查找 U 盘其他分区（标签不是 Ventoy 或 VOTEFI），直到查找到含有 `.live` 目录的分区（且满足上文文件系统）为止。
2. 在本程序运行时，权限为 root，所有挂载的分区为只读挂载。相关内容我只对目录、文件进行了测试，尚不知道对于链接等文件是否会产生不良副作用。`.live` 下的所有文件必须为常规文件或目录，不能为链接等。
3. 使用第一个功能时，不能在某个位置以文件覆盖目录，或者以目录覆盖文件。
4. 三个功能依次进行，当且仅当能探测到所需文件时才运行。
5. 此工具位置为：`/usr/bin/mlt`，为静态编译程序。
6. 开机运行此工具时，`.live` 的位置是 `/tmp/mountpoint/.live`（编写脚本时，如有需要，可以参考）

### 5.4工具源码

[GitHub](https://github.com/KZ25T/My-Live-Tools)，[Gitee](https://gitee.com/KZ25T/my-live-tools)

## 6.安装系统

如您很喜欢我的系统，那么可以尝试安装它。您需要有一定的 Linux 知识以安装它，否则建议仍然按照[官网](https://www.debian.org/distrib/)并参考作者所写的[注意事项](https://blog.csdn.net/m0_57309959/article/details/135856767)安装。

**提醒：** 该功能尚未完善，未经测试，如果因操作不当造成损坏，请自己承担责任。

步骤：

- 启动本操作系统。
- 启动 GParted 给硬盘分区，给安装本系统划分出硬盘空间，并挂载到 `/mnt/debian` 上。（当然挂载点随意）
  - 提示：如果你的电脑只安装有 windows（windows 10 或 11），那么硬盘上应该会有一个 ESP 分区（用 gparted 打开，上边显示标识为 boot, esp 等，格式是 fat32），需要挂载到 `/mnt/debian/boot/efi` 上。
- 复制整个系统启动部分以外的内容：`sudo cp -rp /run/live/rootfs/filesystem.squashfs/* /mnt/debian`
- 安装内核
  - 首先完成 chroot：`sudo chroot /mnt/debian /bin/bash`
  - 然后用 apt 重装：`(chroot) sudo apt reinstall linux-image-amd64`
- 安装 grub 并配置引导
  - 如果您的电脑已经有 grub 了（这种常见于你的电脑已经有一个 linux 操作系统，启动时会进入 grub 页面），那么您不需要安装 grub，只需要按回到原有的 linux 系统，执行 `sudo update-grub`（有些发行版为 `grub-mkconfig -o /boot/grub/grub.cfg`）即可（记得启用 os-prober）
  - 如果您的电脑没有 grub （这种常见于你的电脑可能只有一个 windows 10/11），此时需要安装 grub：
    - 安装 EFI 目录：`(chroot) sudo mkdir -p /boot/efi/EFI/debian`
    - 安装系统探测：`(chroot) sudo apt install os-prober`，然后屏蔽选项：`(chroot) sudo vim /etc/default/grub` 将 `GRUB_DISABLE_OS_PROBER` 设置为 false 或注释掉（否则不能检测到其他系统）
    - 退出 chroot：按 `Ctrl+D` 或 `exit`
    - 安装 grub 文件：`(chroot) sudo cp /usr/lib/grub/x86_64-efi/monolithic/grubx64.efi /boot/efi/EFI/debian`
    - 更新 grub：`(chroot) sudo grub-install 你的硬盘`，之后 `sudo update-grub`
- 设置挂载系统：
  - 通过 blkid 列出各分区的 UUID
  - 设置到 `/etc/fstab` 里，如：

    ```text
    UUID=xxxxxxxx-xxxx-xxxx-xxxx-xxxx /          ext4    errors=remount-ro 0 1
    UUID=xxxx-yyyy                    /boot/efi  vfat    umask=0077        0 1
    ```

- 重启后安装必要软件、重置密码、取消自动登录等。

## 7.联系作者

邮箱：`ikun@mail.ustc.edu.cn`

本系统的部分制作方法，作者已公开于 CSDN 网站：[部分内容](https://blog.csdn.net/m0_57309959)

如您有作者的 QQ 或微信等联系方式，也可以直接联系。
