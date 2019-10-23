1.osdrv 顶层 Makefile 使用说明
本目录下的编译脚本支持选用下文提到的两种工具链中的任何一种进行编译，因此编译时需要带上一个编译参数以指定对应的工具链。其中，arm-hisiv610-linux工具链为32bit工具链，aarch64-hisiv610-linux工具链为64bit工具链。具体命令如下

(1)编译整个osdrv目录：

	编译64bit系统，服务器需要升级mkimage工具，在本发布包的tools/pc/mkimage_tool目录下有可用的mkimage工具，拷贝到服务器的/usr/bin/目录下即可。
	
	flash器件为spi（spi nor和spi nand）的内核和文件系统的命令：
	make FLASH_TYPE=spi all
	或
	make all

	flash器件为nand的内核和文件系统的命令：
	make FLASH_TYPE=nand all

参数说明：
FLASH_TYPE:spi nor和spi nand器件选择spi参数；nand器件选择nand参数。
注意：
big-little内核编译为内存文件系统, 不需要执行FLASH器件。

(2)清除整个osdrv目录的编译文件：
	make clean

(3)彻底清除整个osdrv目录的编译文件，除清除编译文件外，还删除已编译好的镜像：
	make distclean

(4)单独编译kernel：
	a.待进入内核源代码opensource/kernel/(单核:linux-3.18.y, 多核:linux-3.18.y_big-little)目录后，执行以下操作
	cp arch/arm64/configs/hi3559av100es_arm64_defconfig .config （编译单核、SPI版本）
	或
	cp arch/arm64/configs/hi3559av100es_arm64_nand_defconfig .config （编译单核、NAND版本）
	或
	cp arch/arm64/configs/hi3559av100es_arm64_big_little_defconfig .config	(编译多核、SPI版本)
	或
	cp arch/arm64/configs/hi3559av100es_arm64_big_little_nand_defconfig .config	(编译多核、NAND版本)
	或
	cp arch/arm64/configs/hi3559av100es_arm64_big_little_slave_defconfig .config  (编译多核、内存文件系统)

	make ARCH=arm64 CROSS_COMPILE=aarch64-hisiv610-linux- menuconfig  //配置

	make ARCH=arm64 CROSS_COMPILE=aarch64-hisiv610-linux- uImage
	
	b.进入opensource/arm-trusted-firmware目录，单核执行mk.sh，多核执行mk-bl.sh，big-little执行mk-salve.sh，在opensource/arm-trusted-firmware/build/hi3559av100es/debug目录下，
	生成的fip.bin(单核), fip_bl.bin(多核)，fip_slave.bin(big-little)文件就是ATF+kernle的镜像uImage_hi3559av100es.

(5)单独编译uboot：
	待进入boot源代码目录后，执行以下操作
	make CROSS_COMPILE=aarch64-hisiv610-linux- hi3559av100es_defconfig (编译SPI版本,启动介质为emmc、ufs、spiflash)
	或
	make CROSS_COMPILE=aarch64-hisiv610-linux- hi3559av100es_nand_defconfig (编译NAND版本,启动介质为nandflash)

	make CROSS_COMPILE=aarch64-hisiv610-linux- -j 20
	
	cp ../../../tools/pc/uboot_tools/reg_info.bin .reg
	
	make ARCH=arm CROSS_COMPILE=arm-hisiv610-linux- u-boot-z.bin
	
	生成的u-boot-hi3559av100es.bin就是64bit的uboot镜像

(6)制作文件系统镜像：
在osdrv/pub/中有已经编译好的文件系统，因此无需再重复编译文件系统，只需要根据单板上flash的规格型号制作文件系统镜像即可。

	spi flash使用jffs2格式的镜像，制作jffs2镜像时，需要用到spi flash的块大小。这些信息会在uboot启动时会打印出来。建议使用时先直接运行mkfs.jffs2工具，根据打印信息填写相关参数。下面以块大小为64KB为例：
	osdrv/pub/bin/pc/mkfs.jffs2 -d osdrv/pub/rootfs_glibc -l -e 0x40000 -o osdrv/pub/rootfs_glibc_256k.jffs2

	nand flash使用yaffs2格式的镜像，制作yaffs2镜像时，需要用到nand flash的pagesize和ecc。这些信息会在uboot启动时会打印出来。建议使用时先直接运行mkyaffs2image工具，根据打印信息填写相关参数。下面以2KB pagesize、4bit ecc为例：
	osdrv/pub/bin/pc/mkyaffs2image100 osdrv/pub/rootfs_glibc osdrv/pub/rootfs_glibc_2k_4bit.yaffs2 1 2

(7)A53UP和A73MP启动流程说明
	a. uboot运行在A53UP上，可以使用HiTools工具进行烧写；
	b. 在uboot命令行下，通过在bootcmd参数中添加bootm amp参数，使big-little内核镜像在A73MP上启动, big-little内核镜像的启动地址为0x52000000; single内核镜像运行在A53UP上, single内核镜像的启动地址为0x42000000；
	c. bootargs和bootcmd参数配置示例如下：
	--------------------------------------------------------------------------------------------
	|      1M       |      16M            |      96M      |    15M       |         32M         |
	|---------------|---------------------|---------------|--------------|---------------------|
	|     boot      |     A53UP kernel    |     rootfs    |    Reserved  |  big-little Kernel  |
	--------------------------------------------------------------------------------------------
	setenv bootargs 'mem=256M console=ttyAMA0,115200 clk_ignore_unused root=/dev/mmcblk0p3 rootfstype=ext4 rw rootwait blkdevparts=mmcblk0:1M(boot),16M(kernel),96M(rootfs)'
	setenv bootcmd 'mmc read 0 0x52000000 40000 10000; bootm amp 0x52000000; mmc read 0 0x42000000 0x800 0x8000;bootm 0x42000000'

(8) LiteOS M7编译及使用说明：
	a. LiteOS M7编译方法
		进入platform/liteos_m7目录后，解压liteos.tgz；
		解压完成后，进入liteos/sample/sample_osdrv/目录下，执行make命令；
		生成的sample.bin即为可用镜像。
	b. 引导LiteOS M7启动
		引导M7启动需要在A53UP uboot命令下进行操作：
		config_m7;                  //执行config_m7命令，进行pll、时钟与remap的配置
		tftp 0x19000000 sample.bin; //把sample.bin加载到0x19000000地址中（即M7的ITCM空间）
		go_m7                       //启动M7的运行


2. 镜像存放目录说明
编译完的image，rootfs等存放在osdrv/pub目录下
pub
│
├─rootfs_glibc_single.tgz------------------------------------------- hisiv610编译出单核的rootfs文件系统压缩包
├─rootfs_glibc_multi-core.tgz--------------------------------------- hisiv610编译出多核的rootfs文件系统压缩包
├─rootfs_glibc_big-little.tgz--------------------------------------- hisiv610编译出big-little的rootfs文件系统压缩包
│
├─rootfs_glibc_single ----------------------------------------------- hisiv610编译出单核的rootfs文件系统
├─rootfs_glibc_multi-core ------------------------------------------- hisiv610编译出多核的rootfs文件系统
├─rootfs_glibc_big-little ------------------------------------------- hisiv610编译出big-little的rootfs文件系统
│
├─image_glibc_single -------------------------------------------------hisiv610编译出单核的镜像文件
│      uImage_hi3559av100es ----------------------------------------- kernel镜像
│      u-boot-hi3559av100es.bin ------------------------------------- u-boot镜像
│      rootfs_hi3559av100es_64k.jffs2 ------------------------------- 64K jffs2 文件系统镜像
│      rootfs_hi3559av100es_128k.jffs2 ------------------------------ 128K jffs2 文件系统镜像
│      rootfs_hi3559av100es_256k.jffs2 ------------------------------ 256K jffs2 文件系统镜像
│      rootfs_hi3559av100es_2k_4bit.yaffs2 -------------------------- yaffs2 文件系统镜像
│      rootfs_hi3559av100es_96M.ext4 -------------------------------- ext4 文件系统镜像
│ 
├─image_glibc_multi-core -------------------------------------------- hisiv610编译出多核的镜像
│      uImage_hi3559av100es ----------------------------------------- kernel镜像
│      u-boot-hi3559av100es.bin ------------------------------------- u-boot镜像
│      rootfs_hi3559av100es_64k.jffs2 ------------------------------- 64K jffs2 文件系统镜像
│      rootfs_hi3559av100es_128k.jffs2 ------------------------------ 128K jffs2 文件系统镜像
│      rootfs_hi3559av100es_256k.jffs2 ------------------------------ 256K jffs2 文件系统镜像
│      rootfs_hi3559av100es_2k_4bit.yaffs2 -------------------------- yaffs2 文件系统镜像
│      rootfs_hi3559av100es_96M.ext4 -------------------------------- ext4 文件系统镜像 
│
├─image_glibc_big-little-------------------------------------------------hisiv610编译出big-little的镜像文件
│      uImage_hi3559av100es_big_little ------------------------------ kernel镜像
│
│
└─bin
    ├─pc
    │      mkfs.jffs2
    │      mkimage
    │      mkfs.cramfs
    │      mkyaffs2image610
    │      mksquashfs
    │      lzma
    │
    ├─board_glibc -------------------------------------------- hisiv610编译出的单板用工具
        ethtool
        flash_erase
        flash_otp_dump
        flash_otp_info
        sumtool
        mtd_debug
        flashcp
        nandtest
        nanddump
        nandwrite
        gdb-arm-hisiv610-linux
    

3.osdrv目录结构说明：
osdrv
├─Makefile -------------------------------------- osdrv目录编译脚本
├─tools ----------------------------------------- 存放各种工具的目录
│  ├─board -------------------------------------- 各种单板上使用工具
│  │  ├─reg-tools-1.0.0 ------------------------- 寄存器读写工具
│  │  ├─hifat --- ------------------------------- FAT文件系统制作工具
│  │  ├─udev-164 -------------------------------- udev工具集
│  │  ├─mtd-utils ------------------------------- flash裸读写工具集
│  │  ├─gdb ------------------------------------- gdb工具
│  │  ├─ethtools -------------------------------- ethtools工具
│  │  └─e2fsprogs ------------------------------- mkfs工具集
│  └─pc ----------------------------------------- 各种pc上使用工具
│      ├─jffs2_tool------------------------------ jffs2文件系统制作工具
│      ├─cramfs_tool ---------------------------- cramfs文件系统制作工具
│      ├─squashfs4.2 ---------------------------- squashfs文件系统制作工具
│      ├─mkimage_tool --------------------------- uImage制作工具
│      ├─nand_production ------------------------ nand量产工具
│      ├─lzma_tool ------------------------------ lzma压缩工具
│      ├─mkyaffs2image -- ----------------------- yaffs2文件系统制作工具
│      └─uboot_tools ---------------------------- uboot镜像制作工具、xls文件及ddr初始化脚本、Hitool烧写工具
├─pub ------------------------------------------- 存放各种镜像的目录
│  ├─image_glibc -------------------------------- 基于hisiv610工具链编译，可供FLASH烧写的映像文件，包括uboot、内核、文件系统
│  ├─bin ---------------------------------------- 各种未放入根文件系统的工具
│  │  ├─pc -------------------------------------- 在pc上执行的工具
│  │  └─board_glibc ----------------------------- 基于hisiv610工具链编译，在单板上执行的工具
│  └─rootfs_glibc.tgz --------------------------- 基于hisiv610工具链编译的根文件系统
├─opensource------------------------------------- 存放各种开源源码目录
│  ├─toolchain ---------------------------------- 存放工具链的目录
│  ├─busybox ------------------------------------ 存放busybox源代码的目录
│  ├─uboot -------------------------------------- 存放uboot源代码的目录
│  └─kernel ------------------------------------- 存放kernel源代码的目录
├─components------------------------------------- 组件
│  ├─pcie_mcc ----------------------------------- pcie组件
│  └─ipcm --------------------------------------- ipcm驱动
└─rootfs_scripts -------------------------------- 存放根文件系统制作脚本的目录

4.注意事项
(1)在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，导致无法编译使用；u-boot或内核下编译后，会有很多符号链接文件，在windows下复制这些源码包, 会使源码包变的巨大，因为linux下的符号链接文件变为windows下实实在在的文件，因此源码包膨胀。因此使用时请注意不要在windows下复制源代码包。
(2)使用某一工具链编译后，如果需要更换工具链，请先将原工具链编译文件清除，然后再更换工具链编译。
(3)编译板端软件
    Hi3559AV100ES具有浮点运算单元和neon。文件系统中的库是采用软浮点和neon编译而成，因此请用户注意，所有Hi3559AV100ES板端代码编译时, 需要在Makefile里面添加选项-mcpu=cortex-a53或mcpu=cortex-a73.cortex-a53。
    如：
    CFLAGS += -mcpu=cortex-a53
    CXXFlAGS +=-mcpu=cortex-a53
其中CXXFlAGS中的XX根据用户Makefile中所使用宏的具体名称来确定，e.g:CPPFLAGS。
