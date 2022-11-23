#include <stdio.h>
#include "pico/stdlib.h"

#include "dht.h"
#include "hardware/i2c.h"
#include "lcd.h"

#define PSDA 0
#define PSCL 1
#define DHT_PIN 15
#define MAX_TIMINGS 85

int main() {
    stdio_init_all();

    i2c_init(i2c_default, 100 * 1000);
	gpio_set_function(PSDA, GPIO_FUNC_I2C);
	gpio_set_function(PSCL, GPIO_FUNC_I2C);
	gpio_pull_up(PSDA);
	gpio_pull_up(PSCL);

	lcd_instance_t instance = {
		.address = i2c_find_first_device(i2c_default),
		.i2c = i2c_default
	};

	lcd_init(&instance, true, false);
	
	dht_t dht;
	dht_init(&dht, DHT11, pio0, DHT_PIN, true);

	while (true) {
        dht_start_measurement(&dht);
		
		float humidity;
		float temperature_c;
		dht_result_t result = dht_finish_measurement_blocking(&dht, &humidity, &temperature_c);

		lcd_clear(&instance);
		if (result == DHT_RESULT_OK) {
			char buf[16] = { 0 };
			sprintf(buf, "%.1f C", temperature_c);
			lcd_puts(&instance, buf);
			sprintf(buf, "%.1f%% humidity", humidity);
			lcd_cursorpos(&instance, 0x40);
			lcd_puts(&instance, buf);

		} else {
			lcd_puts(&instance, "DHT sensor not responding.");
		}

		sleep_ms(2000);
    }
}
