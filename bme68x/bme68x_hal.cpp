#include <stdio.h>
#include "bme68x_defs.h"
#include "bme68x_hal.h"
#include "hardware/i2c.h"

int write_blocking(uint8_t addr, const uint8_t *src, size_t len, bool nostop) {
        return i2c_write_blocking(i2c_default, addr, src, len, nostop);
    }

    int read_blocking(uint8_t addr, uint8_t *dst, size_t len, bool nostop) {
        return i2c_read_blocking(i2c_default, addr, dst, len, nostop);
    }


 int write_bytes(uint8_t reg_addr, uint8_t *reg_data, uint32_t length) {
    
                uint8_t buffer[length + 1];
                buffer[0] = reg_addr;
                for(uint32_t x = 0u; x < length; x++) {
                    buffer[x + 1] = reg_data[x];
                }

                int result = write_blocking(BME68X_I2C_ADDR_LOW, buffer, length + 1, false);

                return result == PICO_ERROR_GENERIC ? 1 : 0;
            };

int read_bytes(uint8_t reg_addr, uint8_t *reg_data, uint32_t length) {
                
                int result = write_blocking(BME68X_I2C_ADDR_LOW, &reg_addr, 1, true);
                result =read_blocking(BME68X_I2C_ADDR_LOW, reg_data, length, false);

                return result == PICO_ERROR_GENERIC ? 1 : 0;
            };

void bme68x_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    busy_wait_us_32(period);
}

void bme68x_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BME68X_OK:

            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            printf("API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            printf("API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            printf("API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}
