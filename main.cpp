#include <stdio.h>
#include <string.h>
#include "sx126x/lora.h"
#include "sx126x/sx126x_hal.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define LED_GPIO 25

Lora *lora;

int main(){

  stdio_init_all();
  #if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
  #warning spi/bme280_spi example requires a board with SPI pins
    puts("Default SPI pins were not defined");
  #else

  sleep_ms(8000);
  gpio_init(LED_GPIO);
  gpio_set_dir(LED_GPIO, 1);
  gpio_put(LED_GPIO, 1);
  sleep_ms(2000);
  printf("[Main] Setting up Lora Chip");
  
  lora = new Lora();
  lora->SetTxEnable();
  printf("[Main] Done");
  char number[] = {0};
while (1)
{
    hal_gpio_put(LED_GPIO, 0);

  char payload[] = "Om namo venkateshayaa";
  lora->SendData(22, payload, strlen(payload));
      sleep_ms(1000);
      lora->ProcessIrq();

  number[0]++;
  lora->CheckDeviceStatus();

  hal_gpio_put(LED_GPIO, 1);
  sleep_ms(2000);
}
#endif

    return 0;
}


