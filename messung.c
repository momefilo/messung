// momefilo Desing
#include <stdio.h>
#include "pico/stdlib.h"
#include "st7735/st7735.h"
#include "hardware/i2c.h"

#define I2C_SDA 16
#define I2C_SCL 17
#define I2C_BUS i2c0
#define I2C_BAUDRATE (10 * 1000)
#define I2C_ADRESS 72

uint16_t read_adc(uint8_t pos){
	uint8_t reg = 0;
	uint8_t txdata[] = {0x01, 0x03, 0x83};//0x83 = 128sps, 0xA3 = 250sps, 0xC3 = 475sps, 0xE3 = 860sps

	// 0x03 -> FSR = 4,096V in single-shot
	txdata[1] = txdata[1] | ((4 + pos) << 4) | 128;
	printf("MSB=0x%x; LSB=0x%x\n", txdata[1],txdata[2]);
	i2c_write_blocking(I2C_BUS, I2C_ADRESS, txdata, 4, false);
	i2c_write_blocking(I2C_BUS, I2C_ADRESS, &reg, 1, false);

	uint16_t sum;
	uint8_t rxdata[2];
	i2c_read_blocking(I2C_BUS, I2C_ADRESS, rxdata, 2, false);
	sum = ((rxdata[0] << 8) | rxdata[1]);
	return (sum);
}
void myi2c_init(){
	i2c_init(I2C_BUS, I2C_BAUDRATE);
	gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
	gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SCL);
	gpio_pull_up(I2C_SDA);
}
int main(){
	stdio_init_all();
	Display_init();
	myi2c_init();
	sleep_ms(100);
	char *text = "Ready";
	uint8_t pos[] = {0,0};
	uint16_t ret;
	ret = read_adc(0);
	printf("wert0=0x%x\n", ret);
	ret = read_adc(1);
	printf("wert1=0x%x\n", ret);
	ret = read_adc(2);
	printf("wert2=0x%x\n", ret);
	ret = read_adc(3);
	printf("wert3=0x%x\n", ret);
	if(ret > 0) writeText16x16(pos, text, 5, false, false);
}
