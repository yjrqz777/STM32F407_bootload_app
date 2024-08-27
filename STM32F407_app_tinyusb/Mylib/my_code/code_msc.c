#include "code.h"
#include "board_api.h"
#include "tusb.h"
#include "board.h"
#include "stdio.h"
void ToggleLed()
{
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
    HAL_Delay(50);
    cdc_main();
}


void OTG_FS_IRQHandler(void) {
  tud_int_handler(0);

}

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void cdc_task(void);

/*------------- MAIN -------------*/
int cdc_main(void) {
//   board_init();

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

//   if (board_init_after_tusb) {
//     board_init_after_tusb();
//   }

  while (1) {
    tud_task(); // tinyusb device task
    // led_blinking_task();
    
    cdc_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}


//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void) {
  // connected() check for DTR bit
  // Most but not all terminal client set this when making connection
  // if ( tud_cdc_connected() )
  {
    // connected and there are data available
    if (tud_cdc_available()) {
      // read data
      char buf[100] = {0};
      uint32_t count = tud_cdc_read(buf, sizeof(buf));
      if (count)
      {
      // tud_cdc_write(buf, count);
      // tud_cdc_write_flush();
      printf("%s", buf);
      fflush(stdout);    //立即把IO缓冲区的内容立刻传给内核，让内核写回磁盘
      }
    }
    if(UART1_Rx_flg)
    {
      // HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
      tud_cdc_write(UART1_Rx_Buf, UART1_Rx_cnt);
      tud_cdc_write_flush();
      UART1_Rx_flg = 0;
      // printf("%s", UART1_Rx_Buf);
      // fflush(stdout);    //立即把IO缓冲区的内容立刻传给内核，让内核写回磁盘
      memset(UART1_Rx_Buf,0,sizeof(UART1_Rx_Buf));
      UART1_Rx_cnt = 0;
    }  
  }
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
  (void) itf;
  (void) rts;

  // TODO set some indicator
  if (dtr) {
    // Terminal connected
  } else {
    // Terminal disconnected
  }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf) {
  (void) itf;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+

// #if CFG_TUSB_OS == OPT_OS_NONE
// volatile uint32_t system_ticks = 0;

// void SysTick_Handler(void) {
//   HAL_IncTick();
//   system_ticks++;
// }

// uint32_t board_millis(void) {
//   return system_ticks;
// }
// #endif

void board_led_write(bool state) {
  GPIO_PinState pin_state = (GPIO_PinState) (state ? LED_STATE_ON : (1 - LED_STATE_ON));
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, pin_state);
}

void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if (HAL_GetTick() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
