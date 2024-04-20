#ifndef BME68X_HAL_H
#define BME68X_HAL_H
int write_blocking(uint8_t addr, const uint8_t *src, size_t len, bool nostop);
int read_blocking(uint8_t addr, uint8_t *dst, size_t len, bool nostop);
int write_bytes(uint8_t reg_addr, uint8_t *reg_data, uint32_t length);
int read_bytes(uint8_t reg_addr, uint8_t *reg_data, uint32_t length);
void bme68x_config();
void bme68x_delay_us(uint32_t period, void *intf_ptr);
void bme68x_check_rslt(const char api_name[], int8_t rslt);
#endif