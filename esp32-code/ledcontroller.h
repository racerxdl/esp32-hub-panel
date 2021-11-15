#pragma once
#define FASTLED_INTERNAL
#include <FastLED.h>  // Used for some mathematics calculations and effects.


#define R1   14
#define G1   15
#define BL1  16
#define R2   17
#define G2   18
#define BL2  19
#define CH_A 21
#define CH_B 22
#define CH_C 23
#define CH_D 25
#define CH_E -1
#define CLK  2
#define LAT  27
#define OE    5


#include <ESP32-VirtualMatrixPanel-I2S-DMA.h>
// Controller State
#define STRING_DISPLAY 0
#define BACKGROUND_ONLY 1
#define BACKGROUND_STRING_DISPLAY 2
#define MODE_CLOCK 3
#define BACKGROUND_MODE_CLOCK 4
#define MODE_BITMAP 5

#define PANEL_RES_X 64 // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32 // Number of pixels tall of each INDIVIDUAL panel module.

#define NUM_ROWS 2 // Number of rows of chained INDIVIDUAL PANELS
#define NUM_COLS 1 // Number of INDIVIDUAL PANELS per ROW
#define PANEL_CHAIN NUM_ROWS*NUM_COLS     // total number of panels chained one to another// Virtual Panl dimensions - our combined panel would be a square 4x4 modules with a combined resolution of 128x128 pixels
#define VPANEL_W PANEL_RES_X*NUM_COLS     // Kosso: All Pattern files have had the MATRIX_WIDTH and MATRIX_HEIGHT replaced by these.
#define VPANEL_H PANEL_RES_Y*NUM_ROWS     //

#define MATRIX_WIDTH VPANEL_W
#define MATRIX_HEIGHT VPANEL_H

// Methods
VirtualMatrixPanel *getMatrix();
void SetupLeds();
void LedLoop();

void SetBrightness(float v);
void SetBackgroundBrightness(float v);
void SetBackgroundColor(CRGB color);
void Clear();

void ResetToBackground();

void SetBackground(int x, int y, CRGB color);
void SetMode(int mode);
void LedPrint(const char *text, CRGB color);
void LedPrintAt(int x, int y, const char *text, CRGB color);
void LedPrintSetColor(CRGB color);
void SetScrollSpeed(int);

int WriteCharAt(int x, int y, uint8_t charToPut, CRGB fgColor);
int WriteCharAt(int x, int y, uint8_t charToPut, CRGB fgColor, CRGB bgColor);

int WriteStringAt(int x, int y, const char *str, CRGB fgColor);
int WriteStringAt(int x, int y, const char *str, CRGB fgColor, CRGB bgColor);

void StartOTA();
void UpdateOTA(int progress);

void SetBitmapLine(uint16_t lineN, uint16_t pixels[64]);
