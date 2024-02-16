// momefilo Desing
#include "../fonts/font16x16.h"
#include "../fonts/font12x12.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "st7735.h"

#define SPI_PORT spi1
#define PIN_SCK	10
#define PIN_SDA	11
#define PIN_A0  12
#define PIN_CS  13
#define PIN_RST 14

uint8_t FgColor[]={0x0, 0xF, 0x4};
uint8_t BgColor[]={0x4, 0x0, 0xF};
uint8_t SeColor[]={0xF, 0xF, 0x0};
uint8_t Width, Height;

static inline void cs_select() {
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_CS, 0);
	asm volatile("nop \n nop \n nop");
}
static inline void cs_deselect() {
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_CS, 1);
	asm volatile("nop \n nop \n nop");
}
void write_cmd(uint8_t *cmd, int len) {
	cs_select();
	spi_write_blocking(SPI_PORT, cmd, 1);
	if(len > 1){
		gpio_put(PIN_A0, 1);
		spi_write_blocking(SPI_PORT, cmd + (uint8_t)1, len - 1);
		gpio_put(PIN_A0, 0);
	}
	cs_deselect();
}
void set_col(int start, int end){
	uint8_t cmd[5];
	cmd[0] = 0x2A;
	cmd[1] = 0x00;
	cmd[2] = start;
	cmd[3] = 0x00;
	cmd[4] = end;
	write_cmd(cmd, 5);
}
void set_row(int start, int end){
	uint8_t cmd[5];
	cmd[0] = 0x2B;
	cmd[1] = 0x00;
	cmd[2] = start;
	cmd[3] = 0x00;
	cmd[4] = end;
	write_cmd(cmd, 5);
}

void write_font16x16(uint8_t *pos, uint8_t zeichen){
	//create buffer for font
	int len = 16*16*3/2+1;
	uint8_t buf[len];
	buf[0] = 0x2C;
	int z = 1; // Schleifen-Zaehlvariable
	//write font in buffer
	for(int i=0; i<32; i=i+1){ // 32 Byte umfasst ein 16x16Bit Zeichen
		for(int k=0; k<7; k=k+2){ // Acht Bits pro Byte
			if(FONT16x16[zeichen*32+i] & (0x80 >> k)){ // ist das Bit gesetzt
				buf[z] = FgColor[0]<<4 | FgColor[1]; // Wird die FG-Farbe geschrieben
				buf[z+1] = FgColor[2]<<4; // wegen der 12Bit-Farbcodierung
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = BgColor[0]<<4 | BgColor[1];
				buf[z+1] = BgColor[2]<<4;
			}
			if(FONT16x16[zeichen*32+i] & ( 0x80 >> (k+1) )){ // wegen der 12Bit-Farbcodierung
				buf[z+1] = buf[z+1] | FgColor[0];
				buf[z+2] = FgColor[1]<<4 | FgColor[2];
			}
			else{
				buf[z+1] = buf[z+1] | BgColor[0];
				buf[z+2] = BgColor[1]<<4 | BgColor[2];
			}
			z=z+3;
		}
	}
	//write buffer to display
	set_col(pos[0],pos[0]+15);
	set_row(pos[1],pos[1]+15);
	write_cmd(buf,len);
}
void write_font12x12(uint8_t *pos, uint8_t zeichen){
	//create buffer for font
	int len = 12*12*3/2+1;
	uint8_t buf[len];
	buf[0] = 0x2C;
	int z = 1; // Schleifen-Zaehlvariable
	//write font in buffer
	for(int i=0; i<18; i=i+1){ // 18 Byte umfasst ein 12x12Bit Zeichen
		for(int k=0; k<7; k=k+2){ // Acht Bits pro Byte
			if(FONT12x12[zeichen*18+i] & (0x80 >> k)){ // ist das Bit gesetzt
				buf[z] = FgColor[0]<<4 | FgColor[1]; // Wird die FG-Farbe geschrieben
				buf[z+1] = FgColor[2]<<4; // wegen der 12Bit-Farbcodierung
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = BgColor[0]<<4 | BgColor[1];
				buf[z+1] = BgColor[2]<<4;
			}
			if(FONT12x12[zeichen*18+i] & ( 0x80 >> (k+1) )){ // wegen der 12Bit-Farbcodierung
				buf[z+1] = buf[z+1] | FgColor[0];
				buf[z+2] = FgColor[1]<<4 | FgColor[2];
			}
			else{
				buf[z+1] = buf[z+1] | BgColor[0];
				buf[z+2] = BgColor[1]<<4 | BgColor[2];
			}
			z=z+3;
		}
	}
	//write buffer to display
	set_col(pos[0],pos[0]+11);
	set_row(pos[1],pos[1]+11);
	write_cmd(buf,len);
}

void clearScreen(){
	int len = Width*Height*3/2+1;
	uint8_t area[len];
	area[0] = 0x2C;
	for(int i=1; i<len-2; i=i+3){
		area[i] = BgColor[0]<<4 | BgColor[1];
		area[i+1] = BgColor[2]<<4 | BgColor[0];
		area[i+2] = BgColor[1]<<4 | BgColor[2];
	 }
	set_col(0, Width-1);
	set_row(0, Height-1);
	write_cmd(area, (len));
}
void Display_init(){
	if(true){//spi setup
		spi_init(SPI_PORT, 100* 1000 * 1000);
		gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
		gpio_set_function(PIN_SDA, GPIO_FUNC_SPI);
		gpio_init(PIN_CS);
		gpio_set_dir(PIN_CS, GPIO_OUT);
		gpio_put(PIN_CS, 1);
		gpio_init(PIN_A0);
		gpio_set_dir(PIN_A0, GPIO_OUT);
		gpio_put(PIN_A0, 0);
		gpio_init(PIN_RST);
		gpio_set_dir(PIN_RST, GPIO_OUT);
		gpio_put(PIN_RST, 1);
	}
	sleep_ms(200);
	if(true){//display init
		uint8_t cmd[2];
		cmd[1] = 0;
		//sleep out
		cmd[0] = 0x11;
		write_cmd(cmd,1);
		sleep_ms(60);
		//display on
		cmd[0]=0x29;
		write_cmd(cmd,1);
		//12-bit colormode
		cmd[0] = 0x3A;
		cmd[1] = 0x03;
		write_cmd(cmd,2);
		sleep_ms(100);
		//flip vertikal&hoizontal
		cmd[0] = 0x36;
		cmd[1] = 0x60;
		write_cmd(cmd,2);
		Width = 160;
		Height = 128;
	}
	clearScreen();
}
void setOrientation(uint8_t ori){
	uint8_t cmd[2];
	cmd[0] = 0x36;
	cmd[1] = 0x60;
	Width = 160;
	Height = 128;
	if(ori > 0){
		cmd[1] = 0;
		Width = 128;
		Height = 160;
	}
	write_cmd(cmd,2);
}
void setSeColor(uint8_t *color){
	for(int i=0; i<3; i++)SeColor[i] = color[i];
}
void setFgColor(uint8_t *color){
	FgColor[0] = color[0];
	FgColor[1] = color[1];
	FgColor[2] = color[2];
}
void setBgColor(uint8_t *color){
	BgColor[0] = color[0];
	BgColor[1] = color[1];
	BgColor[2] = color[2];
}
void writeText16x16(uint8_t *pos, char *text, int len, bool sel, bool matrix){
	uint8_t tmp_color[3];
	if(sel){
		for(int i=0; i<3; i++){
			tmp_color[i] = FgColor[i];
			FgColor[i] = SeColor[i];
		}
	}
	for(int i=0; i<len; i++){
		uint8_t mypos[2];
		if(matrix){
			mypos[0] = (pos[0]+i)*16;
			mypos[1] = pos[1]*16;
		}
		else{
			mypos[0] = pos[0];
			mypos[1] = pos[1];
			if(i>0) mypos[0] = mypos[0] + i*16;
		}
		write_font16x16(mypos, text[i]);
	}
	if(sel)
		for(int i=0; i<3; i++) FgColor[i] = tmp_color[i];
}
void writeText12x12(uint8_t *pos, char *text, int len, bool sel, bool matrix){
	uint8_t tmp_color[3];
	if(sel){
		for(int i=0; i<3; i++){
			tmp_color[i] = FgColor[i];
			FgColor[i] = SeColor[i];
		}
	}
	for(int i=0; i<len; i++){
		uint8_t mypos[2];
		if(matrix){
			mypos[0] = (pos[0]+i)*12;
			mypos[1] = pos[1]*12;
		}
		else{
			mypos[0] = pos[0];
			mypos[1] = pos[1];
			if(i>0) mypos[0] = mypos[0] + i*12;
		}
		write_font12x12(mypos, text[i]);
	}
	if(sel)
		for(int i=0; i<3; i++) FgColor[i] = tmp_color[i];
}
void paintRect(uint8_t *area, uint8_t *color){
	int len = (area[2] - area[0] + 1)*(area[3] - area[1] + 1)*3/2+1;
	uint8_t pixarea[len];
	area[0] = 0x2C;
	for(int i=1; i<len-2; i=i+3){
		pixarea[i] = color[0]<<4 | color[1];
		pixarea[i+1] = color[2]<<4 | color[0];
		pixarea[i+2] = color[1]<<4 | color[2];
	}
	set_col(area[0], area[2]);
	set_row(area[1], area[3]);
	write_cmd(pixarea, (len));
}
void drawRect(uint8_t *area, uint8_t *data){
	int len = (area[2] - area[0] + 1)*(area[3] - area[1] + 1)*3/2+1;
	set_col(area[0], area[2]);
	set_row(area[1], area[3]);
	write_cmd(data, (len));
}
void paintRectGradient(uint8_t *area, uint16_t color1, uint16_t color2){
	uint8_t cmd[2];
	cmd[0] = 0x3A;
	cmd[1] = 0x05;
	write_cmd(cmd,2);
	int width = (area[2] - area[0]) + 1;
	int height = (area[3] - area[1]) + 1;
	int len = width*height*2+1;
	uint8_t pixarea[len];
	pixarea[0] = 0x2C;

	int8_t red_diff = (((color2 & 0xF800) >> 11) - ((color1 & 0xF800) >> 11));
	float red_summand = red_diff / (float)width;
	float red = ((color1 & 0xF800) >> 11);
	int8_t green_diff = (((color2 & 0x07E0) >> 5) - ((color1 & 0x07E0) >> 5));
	float green_summand = green_diff / (float)width;
	float green = ((color1 & 0x07E0) >> 5);
	int8_t blue_diff = ((color2 & 0x001F)-(color1 & 0x001F));
	float blue_summand = blue_diff / (float)width;
	float blue = ((color1 & 0x001F));

	uint count = 0;
	for(int i=1; i<len-2; i=i+2){
		uint16_t color = ((uint16_t)red << 11) | ((uint16_t)green << 5) | (uint16_t)blue;
		pixarea[i] = (color & 0xFF00)>>8;
		pixarea[i+1] = (color & 0x00FF);
		red = red + red_summand;
		green = green + green_summand;
		blue = blue + blue_summand;
		count++;
		if(count >= (width)){
			count = 0;
			red = (color1 & 0xF800) >> 11;
			green = (color1 & 0x07E0) >> 5;
			blue = (color1 & 0x001F);
		}
	 }
	set_col(area[0], area[2]);
	set_row(area[1], area[3]);
	write_cmd(pixarea, (len));
	cmd[0] = 0x3A;
	cmd[1] = 0x03;
	write_cmd(cmd,2);
}
