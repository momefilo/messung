//momefilo Desing
#ifndef momefilo_st7735_h
#define momefilo_st7735_h 1

#include "pico/stdlib.h"

#define HORIZONTAL 0
#define VERTICAL 1

/* inits the display */
void Display_init();

/* Setzt die Orientierung, Parameter sind HORIZONTAL und VERTICAL */
void setOrientation(uint8_t ori);
/* 12-Bit Farbwert {0x0..0xF Red, 0x0..0xF Green, 0x0..0xF blue}
 * Selektions-Textfarbe */
void setSeColor(uint8_t *color);

/* 12-Bit Farbwert {0x0..0xF Red, 0x0..0xF Green, 0x0..0xF blue}
 * Textfarbe */
void setFgColor(uint8_t *color);
/* 12-Bit Farbwert {0x0..0xF Red, 0x0..0xF Green, 0x0..0xF blue}
 * Hintergrundfarbe */
void setBgColor(uint8_t *color);

/* Fuellt das Display mit der Hintergrundfarbe */
void clearScreen();

/* Schreibt char *text
 * pos[0]: ist die Spalte, pos[1] die Zeile.
 * text: der der Text der angezeigt wird
 * len: ist die anzahl der Zeichen des Textes
 * sel: wenn true wird der Text statt in FGground in SelColor angezeigt.
 * matrix: wenn true dann beziehen sich die Koordinatn in pos auf eine Matrix
 * deren Felder Quadrate mit der Kantelänge von 16 Pixeln sind. */
void writeText16x16(uint8_t *pos, char *text, int len, bool sel, bool matrix);

/* Schreibt char *text
 * pos[0]: ist die Spalte, pos[1] die Zeile.
 * text: der der Text der angezeigt wird
 * len: ist die anzahl der Zeichen des Textes
 * sel: wenn true wird der Text statt in FGground in SelColor angezeigt.
 * matrix: wenn true dann beziehen sich die Koordinatn in pos auf eine Matrix
 * deren Felder Quadrate mit der Kantelänge von 12 Pixeln sind. */
void writeText12x12(uint8_t *pos, char *text, int len, bool sel, bool matrix);

/* zeichnet die 4bit-farbcodierte (RRRRGGGGBBBB) Farbe in den
 * durch area bezeichneten Bereich
 * area[0]: position x.start, area[1] position y.start
 * area[12] position x.end, area[3] position y.end
 * *coloe: color[0]=red, color[1]=green, color[2]=blue */
void paintRect(uint8_t *area, uint8_t *color);

/* zeichnet die 4bit-farbcodierten (RRRRGGGGBBBB) Daten in den
 * durch area bezeichneten Bereich
 * area[0]: position x.start, area[1] position y.start
 * area[12] position x.end, area[3] position y.end
 * *data: die Daten die gezeichnet werden */
void drawRect(uint8_t *area, uint8_t *data);

/* zeichnet einen horizontalen Verlauf von color2 zu color2
 * durch "setOrientation(HORIZONTAL/VERTICAL) kann der Verlauf auch vertikal erfolgen
 * die Farbwerte sind 16bit RRRRRGGGGGGBBBBB codiert*/
void paintRectGradient(uint8_t *area, uint16_t color1, uint16_t color2);
#endif
