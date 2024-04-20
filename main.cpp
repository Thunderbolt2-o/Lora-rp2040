#include "main.h"

#define SAMPLE_COUNT  UINT16_C(300)
#define SCL_PIN 13
#define SDA_PIN 12
#define packet "The BME680 data Temperature %.2f : Pressure %.2f : Humidity %.2f : Gas resistance %.2f"

#define LED_GPIO 25

Lora *lora;

int main(){

  stdio_init_all();
    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    struct bme68x_dev bme;
    bme.intf = BME68X_I2C_INTF;
    bme.read = (bme68x_read_fptr_t)read_bytes;
    bme.write = (bme68x_write_fptr_t)write_bytes;
    bme.delay_us = (bme68x_delay_us_fptr_t)bme68x_delay_us;
    bme.amb_temp = 25;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data;
    uint32_t del_period;
    uint32_t time_ms = 0;
    uint8_t n_fields;
    uint16_t sample_count = 1;
    if (bme68x_init(&bme) != BME68X_OK) {
    char bme_msg[] = "BME680 Initialization Error\r\n";
    printf(bme_msg);
    } else {
    char bme_msg[] = "BME680 Initialized and Ready\r\n";
    printf("Driver code is connected \n");
    printf(bme_msg);
    }
        conf.filter = BME68X_FILTER_OFF;
        conf.odr = BME68X_ODR_NONE;
        conf.os_hum = BME68X_OS_16X;
        conf.os_pres = BME68X_OS_1X;
        conf.os_temp = BME68X_OS_2X;
        rslt = bme68x_set_conf(&conf, &bme);
        bme68x_check_rslt("bme68x_set_conf", rslt);

        heatr_conf.enable = BME68X_ENABLE;
        heatr_conf.heatr_temp = 300;
        heatr_conf.heatr_dur = 100;
        rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);
        bme68x_check_rslt("bme68x_set_heatr_conf", rslt);

  sleep_ms(2000);
  gpio_init(LED_GPIO);
  gpio_set_dir(LED_GPIO, 1);
  gpio_put(LED_GPIO, 1);
  sleep_ms(2000);
  printf("[Main] Setting up Lora Chip");
  
  lora = new Lora();
  lora->SetTxEnable();
  printf("[Main] Done");
while (1)
{
    hal_gpio_put(LED_GPIO, 0);

   rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
            bme68x_check_rslt("bme68x_set_op_mode", rslt);

            /* Calculate delay period in microseconds */
            del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
            bme.delay_us(del_period, bme.intf_ptr);

            time_ms = time_us_32();

            /* Check if rslt == BME68X_OK, report or handle if otherwise */
            rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
            bme68x_check_rslt("bme68x_get_data", rslt);

  char payload[100];
  sprintf(payload, packet, data.temperature, data.pressure, data.humidity, data.gas_resistance);
  lora->SendData(22, payload, strlen(payload));
      sleep_ms(1000);
      lora->ProcessIrq();
  lora->CheckDeviceStatus();

  hal_gpio_put(LED_GPIO, 1);
  sleep_ms(2000);
}

    return 0;
}

