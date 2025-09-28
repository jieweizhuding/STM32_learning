BKP断电不丢失数据，而是由备用电源与外部晶振提供运行的必要条件，当备用电源也掉电时，则BKP的DR清零。

```C++
//需要的头文件为：
    #include "stm32f10x.h"
    #include "stm32f10x_rcc.h"
    #include "stm32f10x_pwr.h"
    #include "stm32f10x_bkp.h"


//BKP的初始化十分简单
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 1. 使能电源接口时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE); // 2. 使能备份接口时钟
    PWR_BackupAccessCmd(ENABLE);                          // 3. 允许访问备份寄存器
    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);            // 4. 向备份寄存器写入数据
    uint16_t data = BKP_ReadBackupRegister(BKP_DR1);     // 5. 从备份寄存器读取数据
```