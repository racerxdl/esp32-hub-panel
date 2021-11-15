#include "ledcontroller.h"
#include "wifi.h"
#include "PatternMaze.h"
#include "PatternSnake.h"

// Change this to your needs, for details on VirtualPanel pls see ChainedPanels example
#define SERPENT false
#define TOPDOWN false

// placeholder for the matrix object
MatrixPanel_I2S_DMA *matrix = nullptr;

// placeholder for the virtual display object
VirtualMatrixPanel  *virtualDisp = nullptr;


VirtualMatrixPanel *getMatrix() {
  return virtualDisp;
}

const uint16_t colorBlack = 0;

void StartOTA() {
  virtualDisp->fillScreen(colorBlack);
  virtualDisp->setTextSize(1);
  virtualDisp->setCursor(5, 5);
  virtualDisp->println("Receiving");
  virtualDisp->setTextColor(matrix->color444(255,255,255), colorBlack);
}

void UpdateOTA(int progress) {
  virtualDisp->setTextSize(2);
  String progressStr = String(progress) + String("%");
  int16_t x1, y1;
  uint16_t w, h;
  virtualDisp->getTextBounds(progressStr, 8, 24, &x1, &y1, &w, &h); //calc width of new string
  virtualDisp->setCursor(32 - (w / 2), 24);
  virtualDisp->print(progressStr);
}

char clockBuffer[8];
CRGB clockColor = CRGB::Red;
int ModeClockLoop(int resetBg) {
  int hours = getHours();
  int minutes = getMinutes();
  int seconds = getSeconds();
  char dots = seconds % 2 ? ':' : ' ';

  memset(clockBuffer, 0x00, 8);
  sprintf(clockBuffer, "%02d%c%02d", hours, dots, minutes);

  virtualDisp->setTextSize(2);

  int16_t x, y;
  uint16_t w, h;
  virtualDisp->getTextBounds(clockBuffer, 0, 0, &x, &y, &w, &h); //calc width of new string
  x = 32 - (w / 2);
  y = 32 - (h / 2);
  
  if (resetBg) {
    Clear();
    return WriteStringAt(x, y, clockBuffer, clockColor, CRGB::Black);
  } else {
    return WriteStringAt(x, y, clockBuffer, clockColor);
  }
}

long lastClockUpdate = 0;
long lastModeUpdate = 0;
int currentMode = STRING_DISPLAY;

PatternMaze maze;
PatternSnake snake;

unsigned int nextEffectUpdate = 0;
void LedLoop() {
  if (currentMode == MODE_CLOCK) {
    if (millis() >= nextEffectUpdate) {
      nextEffectUpdate = snake.drawFrame() + millis();
      ModeClockLoop(0);
    }
  }
  if (currentMode == MODE_BITMAP) {
    if (millis() - lastModeUpdate > 5000) { // After five seconds, switch back to clock
      SetMode(MODE_CLOCK);
      Clear();
    }
  }
}

void SetBitmapLine(uint16_t lineN, uint16_t pixels[64]) {
  SetMode(MODE_BITMAP);
  for (int i = 0; i < 64; i++) {
    virtualDisp->drawPixel(i,lineN, pixels[i]);
  } 
}

void SetBrightness(float v) {
  
}

void SetBackgroundBrightness(float v) {
  
}

void SetBackgroundColor(CRGB color) {
  
}

void Clear() {
  virtualDisp->fillScreen(virtualDisp->color444(0, 0, 0));
}

void ResetToBackground() {
  virtualDisp->fillScreen(virtualDisp->color565(0, 0, 0));
}

void SetBackground(int x, int y, CRGB color) {
  
}

void SetMode(int mode) {
  currentMode = mode;
  lastModeUpdate = millis();
}

void LedPrint(const char *text, CRGB color) {
  LedPrintAt(0, 0, text, color);
}

void LedPrintAt(int x, int y, const char *text, CRGB color) {
  WriteStringAt(x, y, text, color, colorBlack);
}

void LedPrintSetColor(CRGB color) {
  virtualDisp->setTextColor(virtualDisp->color444(color.red,color.green,color.blue));
}

void SetScrollSpeed(int) {
  
}

int WriteCharAt(int x, int y, uint8_t charToPut, CRGB fgColor) {
  virtualDisp->setCursor(x, y);
  virtualDisp->setTextColor(virtualDisp->color444(fgColor.red,fgColor.green,fgColor.blue));
  virtualDisp->print((char) charToPut);
  return 0;
}

int WriteCharAt(int x, int y, uint8_t charToPut, CRGB fgColor, CRGB bgColor) {
  virtualDisp->setCursor(x, y);
  virtualDisp->setTextColor(virtualDisp->color444(fgColor.red,fgColor.green,fgColor.blue), virtualDisp->color444(fgColor.red,fgColor.green,fgColor.blue));
  virtualDisp->print((char) charToPut);
  return 0;
}

int WriteStringAt(int x, int y, const char *str, CRGB fgColor) {
  virtualDisp->setCursor(x, y);
  virtualDisp->setTextColor(virtualDisp->color444(fgColor.red,fgColor.green,fgColor.blue));
  virtualDisp->print(str);

  return 0;
}

int WriteStringAt(int x, int y, const char *str, CRGB fgColor, CRGB bgColor) {
  virtualDisp->setCursor(x, y);
  virtualDisp->setTextColor(virtualDisp->color444(fgColor.red,fgColor.green,fgColor.blue), virtualDisp->color444(fgColor.red,fgColor.green,fgColor.blue));
  virtualDisp->print(str);

  return 0;
}


void SetupLeds() {
    // Configure your matrix setup here
  HUB75_I2S_CFG mxconfig(PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN);

  // custom pin mapping (if required)
  HUB75_I2S_CFG::i2s_pins _pins={R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK};
  mxconfig.gpio = _pins;

  // OK, now we can create our matrix object
  matrix = new MatrixPanel_I2S_DMA(mxconfig);

  // let's adjust default brightness to about 75%
  matrix->setBrightness8(64);    // range is 0-255, 0 - 0%, 255 - 100%
  if( not matrix->begin() )
      Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
  
  // create VirtualDisplay object based on our newly created dma_display object
  virtualDisp = new VirtualMatrixPanel((*matrix), NUM_ROWS, NUM_COLS, PANEL_RES_X, PANEL_RES_Y, SERPENT, TOPDOWN);
  virtualDisp->fillScreen(colorBlack);
  for (int i = 0; i < 64; i++) {
    virtualDisp->drawPixel(i,i, matrix->color565(0,0,255));
    virtualDisp->drawPixel(63-i,i, matrix->color565(0,0,255));
  }
  virtualDisp->setTextSize(1);
  virtualDisp->setTextColor(matrix->color444(255,255,255), colorBlack);
  effects.Setup();
  snake.start();
}
