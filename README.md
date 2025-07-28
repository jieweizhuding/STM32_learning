main分支为模板分支，除了需要修改模板，否则不再发展。其余内容在对应分支上实现对main分支的发展。


该项目使用的库为STM32F103C8T6官方标准库。

构建与收录过程指令为：

一切命令在build文件夹中执行。

cmake工具工具链设置：
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/gcc-arm-none-eabi.cmake ..

编译：make

烧录：
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program GPIO.hex verify reset exit"
