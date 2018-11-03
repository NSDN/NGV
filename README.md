# NyaGame Vita 单板游戏机

`Copyright (C) NSDN 2014 - 2019`

## 项目简介

`本项目是NyaGame系列计划下的子项目`

### 关于NyaGame

NyaGame是围绕同人作品《东方Project》二次创作的软硬件周边开发团队。本团队围绕《东方Project》同人二次创作进行相关的软硬件产品开发。本团队为国内最大的东方Project社区喵玉殿论坛旗下的开发机构，隶属于喵玉殿技术部。目前主要进行东方Project相关同人游戏开发和同人网络平台的开发、运维业务。

### 关于NyaGame Vita

NyaGame Vita 单板游戏机（简称 NGV）是NyaGame Portable 单板游戏机（简称 NGP）项目进行到中期的时候确立的新项目。主要设计思路是双核心架构，即使用 ARM(TM) Cortex(R)-M 单片机作为主核心，FPGA 作为时序敏感外设控制器、外部存储控制器和图形处理器。单片机侧运行字节码脚本引擎或脚本语言，以抽象各个硬件资源。

这样的单板电子设备还未出现在市面上，当前的相关产品要么是纯单片机系统，要么是 ARM(TM) Cortex(R)-A 系统，对于开发人员而言，在可玩性和自由度上可能还有所欠缺。这种硬核（Hardcore）的设计大多是兴趣驱动，项目本身也不包含太大的商业价值，项目进行过程中的技术积累可能会远远高于商业价值，这些技术的潜在商业价值可能也是存在的。

NGV 的主要研发工作是FPGA侧的图形处理器架构和实现，这片代号为 99-8B 的芯片暂规划为三个研发阶段：

    第一阶段是承担原本单片机进行的屏幕时序操作，可概括为器件初始化和配置、像素流传输、单一像素重复传输；
    
    第二阶段是双核心共享内存区并进行内存到屏幕的像素流传输，及多图层混合等，此时FPGA侧需要设计专有的图形处理器指令集，以实现点阵字库文字绘制、点阵数据图像渲染和绘制、背景和前景的混合和绘制；  
    
    第三阶段是并行化渲染流水线设计，图元等概念的引入，指令集的扩充，指令缓存的实现等，第三阶段将会把显示屏接口调整为RGB888，由FPGA侧提供同步和扫描时钟。
    
远期的规划可能会引入部分3D渲染功能，目前暂不考虑。

除此之外，99-8B 还包含双通道音频输出单元，当然这里输出的是方波信号，用于驱动蜂鸣器。远期可能会考虑配置 SAI 接口的音频 AD/DA 用于音频播放。

考虑到 FPGA 侧的引脚资源有限，此单板游戏机的按键均连接至单片机，方向键为矩阵结构，功能键为独立按键。

单片机和 FPGA 通过 FMC 总线进行连接，二者共享一片 16 位宽，32MB 容量的 SDRAM，这存储器除了用于单片机系统 RAM 扩展以外，也作为贴图及字库等数据的缓存，便于 FPGA 的读取及显示。

除此之外，单片机和 FPGA 都分别配置了一片 SPI Flash 存储器，单片机侧也提供了 Micro SD 接口，存储器资源较为丰富。

NGV 第一版为双层板，尺寸 210mm x 90mm，板外形酷似任天堂 Switch。当然了，NGV 和 NS 的差别还是相当大的。第一版选用的单片机为 STM32F429ZGT6，配置 W25Q128 存储器，FPGA 选用的是国产的 AL3S10LG144，其内部 3D 封装了 32 位宽，2MB 容量的 SDRAM，FPGA 侧配置 W25Q16 用于存储 FPGA 配置码流、GD25Q16 用于存储额外数据。屏幕使用了一块 854 x 480 分辨率的 IPS 显示屏，大小为 5 英寸，配备 I2C 接口的电容触摸屏。

#### 软件部分

主要是 STM32 HAL 和 FPGA 侧的相关驱动代码组合得到 BSP 层，将 BSP 层应用到 BIOS 层、Bootloader 层和 VM 层，也将 BSP 层部分功能接入到脚本语言中（目前使用的是 NSASM）。用户只需要接触 VM 和脚本语言便能方便使用各类硬件资源。同时我们将 BSP 层进行移植就可以开发出运行在其他平台上的模拟器程序，目前已开发出了基于 GCC 的模拟器程序 ngv-emu，前期显示驱动和脚本语言的开发和调试便在这模拟器上进行。

## 项目版权

FPGA 侧部分自研 IP 核将进行闭源，其他部分按开源项目执行。

目前项目涉及到的部分子项目和关联项目已准备进行著作权登记。

## 等待更新 ...

## 联系我们

NSDN QQ Group: 398529460