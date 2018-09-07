ngv-emu
    NyaGame Vita单板游戏机模拟器
    https://github.com/NSDN/NGV/tree/master/cortex/emulator

基于gcc8.1编译，图形部分依赖SDL2，应该只有64位系统才能运行

1. 双击ngv-emu.exe后可见两个窗口，一个为命令行窗口，一个为SDL图形窗口
   前者模拟的是串口输入输出的部分，后者则是显示屏和物理按键
2. 程序显示完提示信息后，显示"Press any key to continue."，需要激活命令行窗口并按任意键
   然后可以进入NSHEL交互式执行环境（一个简单的shell）
3. 可按上文提到的提示信息自行在命令行窗口中操作，并观察SDL窗口的变化
   也可按以下步骤进行体验
4. 在命令行窗口键入 hello.e 并回车，观察输出，然后在命令行窗口按任意键返回
5. 在命令行窗口键入 sam.ns 并回车，观察输出，待提示输入值时，在命令行窗口选择输入 0 或者 1 并回车，观察输出
6. 在命令行窗口键入 test.ns 并回车，观察输出
7. 在命令行窗口键入 sample.nsg 并回车，在SDL窗口观察变化，待SDL窗口提示按 F8 键时，按下对应键完成测试
8. 在命令行窗口键入 exit 并回车，观察输出及SDL窗口的变化，然后就可返回到步骤2
9. 在命令行窗口按下 Ctrl + C 或者直接关闭两个窗口的任意一个，或者在SDL窗口中按 ESC 都可以关闭程序

Assets目录下：
    hello.e 为NSHEL脚本，可以直接键入文件名执行
    init.d 为启动脚本，由NSHEL执行
    logo.bmp 为SDL窗口图标
    NGV_INFO.TXT 为信息文件
    p1s.ns 和 sam.ns 为NSASM脚本，二者组合在一起进行特性测试，请勿直接执行 p1s.ns
    sample.nsg 为NSGDX脚本，可以直接键入文件名执行
    test.ns 为NSASM脚本，用于测试其他特性

以上脚本均可使用文本编辑器打开并编辑，部分关于NSASM的信息可参考以下链接：
    https://github.com/NSDN/NSASM
