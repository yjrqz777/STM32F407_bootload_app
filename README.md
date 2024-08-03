# st32f047的bootload和app的makefile代码



# cubemx生成 


# bootload 请修改makefile 的 41行

```
C_SOURCES =  \
Core/Src/bootload.c \
```

STM32F407VETx_FLASH.ld 的 66行
```
FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 512K
```

system_stm32f4xx.c 的108行
```
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
```

# APP 请修改makefile 的 41行

```
C_SOURCES =  \
Core/Src/main.c \
```

STM32F407VETx_FLASH.ld 的 66行
```
FLASH (rx)      : ORIGIN = 0x8008000, LENGTH = 448K
```

system_stm32f4xx.c 的108行
```
#define VECT_TAB_OFFSET         0x00008000U     /*!< Vector Table base offset field.
```


# 编译两次下载HEX文件 推荐使用 cubemx programmer