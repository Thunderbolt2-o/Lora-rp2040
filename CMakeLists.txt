add_executable(code main.cpp 
            bme68x/bme68x_hal.cpp bme68x/bme68x.c 
            sx126x/sx126x.c sx126x/sx126x_hal.c sx126x/lora.cpp 
            pico-ssd1306/ssd1306.cpp pico-ssd1306/textRenderer/TextRenderer.cpp 
            pico-ssd1306/frameBuffer/FrameBuffer.cpp)

target_link_libraries(code pico_stdlib hardware_i2c hardware_spi)

pico_enable_stdio_usb(code 1)
pico_enable_stdio_uart(code 0)

pico_add_extra_outputs(code)