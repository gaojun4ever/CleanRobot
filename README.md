###一、工程结构

- USER
- CORE
- FWLIB

###二、工程配置

1、USER文件下添加 main.c、stm32f4xx_it.c、system_stm32f4xx.c
2、CORE文件下添加 startup_stm32f40_41xxx.s
3、FWWLIB添加STM32F4标准库

- Define: STM32F40_41xxx,USE_STDPERIPH_DRIVER

####Include Path:
- ..\Source_Code\FWLIB\inc
- ..\Source_Code\CORE\inc
- ..\Source_Code\USER\inc
