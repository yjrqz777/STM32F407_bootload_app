#ifndef CODE_H
#define CODE_H

#include "main.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

void ToggleLed();
void cdc_OTG_FS_IRQHandler(void);

#endif