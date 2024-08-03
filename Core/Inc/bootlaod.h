#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include "stm32f4xx_hal.h"
#include "usart.h"

typedef  void (*pFunction)(void);
#define APPLICATION_ADDRESS   (uint32_t)0x08008000 


#endif 

