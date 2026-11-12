#include <Wire.h>
#include <SPI.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/dma.h>

#define TFT_MOSI 11
#define TFT_SCK 10
#define TFT_CS 15
#define TFT_DC 16
#define TFT_RST 17
#define TFT_BL 18

#define BUTTON_SELECT 13
#define BUTTON_MAIN 14

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

#define I2C_SDA 4
#define I2C_SCL 5

#define RTC_ADDRESS 0x6F

#define SCREEN_COUNT 5
#define QUOTES_NUMBER 98

// ST7789 Initialization Commands
#define ST7789_PORCH 0xB2
#define ST7789_RAMCTRL 0xB0
#define ST7789_RGBCTRL 0xB1
#define ST7789_TEOFF 0x34
#define ST7789_TEON 0x35
#define ST7789_MADCTL 0x36
#define ST7789_PIXEL 0x3A
#define ST7789_GCTRL 0xB7
#define ST7789_VCOMSET 0xBB
#define ST7789_LCMCTRL 0xC0
#define ST7789_VDVVRHEN 0xC2
#define ST7789_VRHS 0xC3
#define ST7789_VDVS 0xC4
#define ST7789_FRCTRL2 0xC6
#define ST7789_PWCTRL1 0xD0
#define ST7789_NOTONSHEET 0xD6
#define ST7789_PVGAMCTRL 0xE0
#define ST7789_NVGAMCTRL 0xE1
#define ST7789_INVOFF 0x20
#define ST7789_INVON 0x21
#define ST7789_DISON 0x29
#define ST7789_SLEEPOUT 0x11
#define ST7789_MEMORYWRITE 0x2C
#define ST7789_COLUMNSET 0x2A
#define ST7789_ROWSET 0x2B

//TwoWire myWire(I2C_SDA, I2C_SCL);

const uint8_t textFont[][5] = {
  {0x00,0x00,0x00,0x00,0x00}, // space
  {0x00,0x00,0x5F,0x00,0x00}, // !
  {0x00,0x07,0x00,0x07,0x00}, // "
  {0x14,0x7F,0x14,0x7F,0x14}, // #
  {0x24,0x2A,0x7F,0x2A,0x12}, // $
  {0x23,0x13,0x08,0x64,0x62}, // %
  {0x36,0x49,0x55,0x22,0x50}, // &
  {0x00,0x05,0x03,0x00,0x00}, // '
  {0x00,0x1C,0x22,0x41,0x00}, // (
  {0x00,0x41,0x22,0x1C,0x00}, // )
  {0x08,0x2A,0x1C,0x2A,0x08}, // *
  {0x08,0x08,0x3E,0x08,0x08}, // +
  {0x00,0x50,0x30,0x00,0x00}, // ,
  {0x08,0x08,0x08,0x08,0x08}, // -
  {0x00,0x60,0x60,0x00,0x00}, // .
  {0x20,0x10,0x08,0x04,0x02}, // /
  {0x3E,0x51,0x49,0x45,0x3E}, // 0
  {0x00,0x42,0x7F,0x40,0x00}, // 1
  {0x42,0x61,0x51,0x49,0x46}, // 2
  {0x21,0x41,0x45,0x4B,0x31}, // 3
  {0x18,0x14,0x12,0x7F,0x10}, // 4
  {0x27,0x45,0x45,0x45,0x39}, // 5
  {0x3C,0x4A,0x49,0x49,0x30}, // 6
  {0x01,0x71,0x09,0x05,0x03}, // 7
  {0x36,0x49,0x49,0x49,0x36}, // 8
  {0x06,0x49,0x49,0x29,0x1E}, // 9
  {0x00,0x36,0x36,0x00,0x00}, // :
  {0x00,0x56,0x36,0x00,0x00}, // ;
  {0x00,0x08,0x14,0x22,0x41}, // <
  {0x14,0x14,0x14,0x14,0x14}, // =
  {0x41,0x22,0x14,0x08,0x00}, // >
  {0x02,0x01,0x51,0x09,0x06}, // ?
  {0x32,0x49,0x79,0x41,0x3E}, // @
  {0x7E,0x11,0x11,0x11,0x7E}, // A
  {0x7F,0x49,0x49,0x49,0x36}, // B
  {0x3E,0x41,0x41,0x41,0x22}, // C
  {0x7F,0x41,0x41,0x22,0x1C}, // D
  {0x7F,0x49,0x49,0x49,0x41}, // E
  {0x7F,0x09,0x09,0x01,0x01}, // F
  {0x3E,0x41,0x41,0x51,0x32}, // G
  {0x7F,0x08,0x08,0x08,0x7F}, // H
  {0x00,0x41,0x7F,0x41,0x00}, // I
  {0x20,0x40,0x41,0x3F,0x01}, // J
  {0x7F,0x08,0x14,0x22,0x41}, // K
  {0x7F,0x40,0x40,0x40,0x40}, // L
  {0x7F,0x02,0x04,0x02,0x7F}, // M
  {0x7F,0x04,0x08,0x10,0x7F}, // N
  {0x3E,0x41,0x41,0x41,0x3E}, // O
  {0x7F,0x09,0x09,0x09,0x06}, // P
  {0x3E,0x41,0x51,0x21,0x5E}, // Q
  {0x7F,0x09,0x19,0x29,0x46}, // R
  {0x46,0x49,0x49,0x49,0x31}, // S
  {0x01,0x01,0x7F,0x01,0x01}, // T
  {0x3F,0x40,0x40,0x40,0x3F}, // U
  {0x1F,0x20,0x40,0x20,0x1F}, // V
  {0x7F,0x20,0x18,0x20,0x7F}, // W
  {0x63,0x14,0x08,0x14,0x63}, // X
  {0x03,0x04,0x78,0x04,0x03}, // Y
  {0x61,0x51,0x49,0x45,0x43}, // Z
  {0x00,0x00,0x7F,0x41,0x41}, // [
  {0x02,0x04,0x08,0x10,0x20}, // 
  {0x41,0x41,0x7F,0x00,0x00}, // ]
  {0x04,0x02,0x01,0x02,0x04}, // ^
  {0x40,0x40,0x40,0x40,0x40}, // _
  {0x00,0x01,0x02,0x04,0x00}, // `
  {0x20,0x54,0x54,0x54,0x78}, // a
  {0x7F,0x48,0x44,0x44,0x38}, // b
  {0x38,0x44,0x44,0x44,0x20}, // c
  {0x38,0x44,0x44,0x48,0x7F}, // d
  {0x38,0x54,0x54,0x54,0x18}, // e
  {0x08,0x7E,0x09,0x01,0x02}, // f
  {0x08,0x14,0x54,0x54,0x3C}, // g
  {0x7F,0x08,0x04,0x04,0x78}, // h
  {0x00,0x44,0x7D,0x40,0x00}, // i
  {0x20,0x40,0x44,0x3D,0x00}, // j
  {0x00,0x7F,0x10,0x28,0x44}, // k
  {0x00,0x41,0x7F,0x40,0x00}, // l
  {0x7C,0x04,0x18,0x04,0x78}, // m
  {0x7C,0x08,0x04,0x04,0x78}, // n
  {0x38,0x44,0x44,0x44,0x38}, // o
  {0x7C,0x14,0x14,0x14,0x08}, // p
  {0x08,0x14,0x14,0x18,0x7C}, // q
  {0x7C,0x08,0x04,0x04,0x08}, // r
  {0x48,0x54,0x54,0x54,0x20}, // s
  {0x04,0x3F,0x44,0x40,0x20}, // t
  {0x3C,0x40,0x40,0x20,0x7C}, // u
  {0x1C,0x20,0x40,0x20,0x1C}, // v
  {0x3C,0x40,0x30,0x40,0x3C}, // w
  {0x44,0x28,0x10,0x28,0x44}, // x
  {0x0C,0x50,0x50,0x50,0x3C}, // y
  {0x44,0x64,0x54,0x4C,0x44}, // z
  {0x00,0x08,0x36,0x41,0x00}, // {
  {0x00,0x00,0x7F,0x00,0x00}, // |
  {0x00,0x41,0x36,0x08,0x00}, // }
  {0x08,0x08,0x2A,0x1C,0x08}, // ~
  {0x08,0x1C,0x2A,0x08,0x08}  // 
};

static int dma_chan = -1;

const unsigned long debounceDelay = 200;

static inline void gpio_init_fast_pin(uint pin, bool out, int af = -1) {
  if (out) gpio_set_dir(pin, GPIO_OUT);
  else gpio_set_dir(pin, GPIO_IN);
}

uint8_t bcdToDec(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t decToBcd(uint8_t dec) {
  return ((dec / 10) << 4) | (dec % 10);
}

uint16_t displayBuffer[TFT_WIDTH * TFT_HEIGHT];

void setMCP7940DateTime(
  uint8_t second,
  uint8_t minute,
  uint8_t hour,
  uint8_t weekday,
  uint8_t day,
  uint8_t month,
  uint8_t year
) {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x00);
  // Seconds register: bit7 = ST (start oscillator)
  Wire.write(decToBcd(second) | 0x80);
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour)); // 24h mode
  // bit3 = VBATEN (enable battery backup)
  Wire.write(decToBcd(weekday) | 0x08);
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}

void readMCP7940DateTime(
  uint8_t &sec,
  uint8_t &min,
  uint8_t &hour,
  uint8_t &wday,
  uint8_t &day,
  uint8_t &month,
  uint8_t &year
) {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(RTC_ADDRESS, 7);

  uint8_t rawSec = Wire.read();   // 0x00
  uint8_t rawMin = Wire.read();   // 0x01
  uint8_t rawHour = Wire.read();   // 0x02
  uint8_t rawWday = Wire.read();   // 0x03
  uint8_t rawDay = Wire.read();   // 0x04
  uint8_t rawMonth = Wire.read();   // 0x05
  uint8_t rawYear = Wire.read();   // 0x06

  sec   = bcdToDec(rawSec & 0x7F);   // bit7 = ST
  min   = bcdToDec(rawMin & 0x7F);
  hour  = bcdToDec(rawHour & 0x3F);  // 24h mode
  wday  = rawWday & 0x07;
  day   = bcdToDec(rawDay & 0x3F);
  month = bcdToDec(rawMonth & 0x1F);
  year  = bcdToDec(rawYear);
}

void sendCommand(uint8_t command) {
  gpio_put(TFT_DC, 0);
  gpio_put(TFT_CS, 0);
  uint8_t b = command;
  spi_write_blocking(spi1, &b, 1);
  gpio_put(TFT_CS, 1);
}

void sendData(uint8_t data) {
  gpio_put(TFT_DC, 1);
  gpio_put(TFT_CS, 0);
  uint8_t b = data;
  spi_write_blocking(spi1, &b, 1);
  gpio_put(TFT_CS, 1);
}

// Configure SPI to 8-bit for commands/data bytes
void spiSet8Bit() {
  spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

// Configure SPI to 16-bit for pixel DMA transfers
void spiSet16Bit() {
  spi_set_format(spi1, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

void initDisplay() {
  gpio_put(TFT_RST, 0);
  delay(50);
  gpio_put(TFT_RST, 1);
  delay(50);

  spiSet8Bit();

  sendCommand(ST7789_MADCTL); // Memory Data Access Control
  //sendData(0x20); // Adjust for rotation
  sendData(0x60);

  sendCommand(ST7789_PIXEL);
  sendData(0x55); // 16-bit color

  sendCommand(ST7789_PORCH);
  sendData(0x0C);
  sendData(0x0C);
  sendData(0x00);
  sendData(0x33);
  sendData(0x33);

  sendCommand(ST7789_GCTRL);
  sendData(0x35);

  sendCommand(ST7789_VCOMSET);
  sendData(0x2B); // Different (0x1F)

  sendCommand(ST7789_LCMCTRL);
  sendData(0x2C);

  sendCommand(ST7789_VDVVRHEN);
  sendData(0x01);

  sendCommand(ST7789_VRHS);
  sendData(0x0F); // Different (0x15);

  sendCommand(ST7789_VDVS);
  sendData(0x20);

  sendCommand(ST7789_FRCTRL2);
  sendData(0x0F);

  sendCommand(ST7789_PWCTRL1);
  sendData(0xA4);
  sendData(0xA1);

  sendCommand(ST7789_PVGAMCTRL);
  sendData(0xD0); 
  sendData(0x04); // sendData(0x0C);
  sendData(0x0D); // sendData(0x13);
  sendData(0x11); // sendData(0x0A);
  sendData(0x13); // sendData(0x0B);
  sendData(0x2B); // sendData(0x05);
  sendData(0x3F); // sendData(0x3A);
  sendData(0x54); // sendData(0x44);
  sendData(0x4C); // sendData(0x51);
  sendData(0x18); // sendData(0x35);
  sendData(0x0D); // sendData(0x11);
  sendData(0x0B); // sendData(0x11);
  sendData(0x1F); // sendData(0x2F);
  sendData(0x23); // sendData(0x33);

  sendCommand(ST7789_NVGAMCTRL);
  sendData(0xD0);
  sendData(0x04); // sendData(0x0E);
  sendData(0x0C); // sendData(0x14);
  sendData(0x11); // sendData(0x0B);
  sendData(0x13); // sendData(0x08);
  sendData(0x2C); // sendData(0x16);
  sendData(0x3F); // sendData(0x3A);
  sendData(0x44); // sendData(0x43);
  sendData(0x51); // sendData(0x50);
  sendData(0x2F); // sendData(0x1A);
  sendData(0x1F); // sendData(0x16);
  sendData(0x1F); // sendData(0x16);
  sendData(0x20); // sendData(0x30);
  sendData(0x23); // sendData(0x35);

  sendCommand(ST7789_INVOFF);
  sendCommand(ST7789_SLEEPOUT);
  delay(120);
  sendCommand(ST7789_DISON);
  // sendCommand(ST7789_MEMORYWRITE);
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  spiSet8Bit();
  // Column set
  gpio_put(TFT_DC, 0); 
  gpio_put(TFT_CS, 0);
  uint8_t cmd = ST7789_COLUMNSET;
  spi_write_blocking(spi1, &cmd, 1);
  gpio_put(TFT_DC, 1);
  uint8_t bufCol[4] = { (uint8_t)(x0>>8), (uint8_t)(x0&0xFF), (uint8_t)(x1>>8), (uint8_t)(x1&0xFF) };
  spi_write_blocking(spi1, bufCol, 4);
  gpio_put(TFT_CS, 1);

  // Row set
  gpio_put(TFT_DC, 0);
  gpio_put(TFT_CS, 0);
  cmd = ST7789_ROWSET;
  spi_write_blocking(spi1, &cmd, 1);
  gpio_put(TFT_DC, 1);
  uint8_t bufRow[4] = { (uint8_t)(y0>>8), (uint8_t)(y0&0xFF), (uint8_t)(y1>>8), (uint8_t)(y1&0xFF) };
  spi_write_blocking(spi1, bufRow, 4);
  gpio_put(TFT_CS, 1);

  // Memory write
  gpio_put(TFT_DC, 0);
  gpio_put(TFT_CS, 0);
  cmd = ST7789_MEMORYWRITE;
  spi_write_blocking(spi1, &cmd, 1);
  gpio_put(TFT_CS, 1);
}

void dmaPushFrame(const uint16_t *frame, uint32_t pixels) {
  spiSet16Bit();
  gpio_put(TFT_DC, 1);
  gpio_put(TFT_CS, 0);
  // Configure DMA channel (we assume channel already claimed)
  //dma_channel_reset(dma_chan);
  dma_channel_config c = dma_channel_get_default_config(dma_chan);
  // transfer 16-bit words from frame[] to SPI DR register (no write increment)
  channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
  channel_config_set_read_increment(&c, true); // advance through the frame buffer
  channel_config_set_write_increment(&c, false); // write to a peripheral register
  // DREW for SPI TX (true => TX)
  channel_config_set_dreq(&c, spi_get_dreq(spi1, true));
  // target is SPI FIFO/data register:
  volatile void *tx_reg = (void *)&(spi_get_hw(spi1)->dr);

  // Configure DMA: destination = SPI DR, source = frame, transfer count = pixels
  dma_channel_configure(dma_chan, &c, tx_reg, frame, pixels, true);
  // Wait for DMA completion
  dma_channel_wait_for_finish_blocking(dma_chan);
  gpio_put(TFT_CS, 1);
} 

void fillBuffer(uint16_t color) {
  for (uint32_t i = 0; i < TFT_WIDTH * TFT_HEIGHT; i++) {
    displayBuffer[i] = color;
  }
}

void pushBuffer() { // Push full buffer to screen
  setAddrWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
  dmaPushFrame(displayBuffer, TFT_WIDTH * TFT_HEIGHT);
} 

void drawPixelToBuffer(uint16_t x, uint16_t y, uint16_t color) {
  if (x < TFT_WIDTH && y < TFT_HEIGHT) {
    displayBuffer[y * TFT_WIDTH + x] = color;
  }
}

void drawCharToBuffer(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
  if (c < 32 || c > 126) return;
  uint8_t idx = c - 32;
  for (uint8_t i = 0; i < 5; i++) {
    uint8_t line = textFont[idx][i];
    for (uint8_t j = 0; j < 7; j++) {
      if (line & (1 << j)) {
        for (uint8_t a = 0; a < size; a++) {
          for (uint8_t b = 0; b < size; b++) {
            drawPixelToBuffer(x + (i * size) + a, y + (j * size) + b, color);
          }
        }
      } else if (bg != color) {
        for (uint8_t a = 0; a < size; a++) {
          for (uint8_t b = 0; b < size; b++) {
            drawPixelToBuffer(x + (i * size) + a, y + (j * size) + b, bg);
          }
        }
      }
    }
  }
}

void drawStringToBuffer(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg, uint8_t size) {
  uint16_t origX = x;
  while (*str) {
    if (*str == '\n') { y += 8 * size; x = origX; str++; continue; }
    drawCharToBuffer(x, y, *str, color, bg, size);
    x += 5 * size + 1;
    str++;
  }
}

void drawWrappedText(
  uint16_t x, uint16_t y,
  const char* text,
  uint16_t maxWidth,
  uint16_t color,
  uint16_t bg,
  uint8_t size
) {
  uint16_t cx = x;
  uint16_t cy = y;

  while (*text) {
    if (*text == '\n') {
      cy += 8 * size;
      cx = x;
      text++;
      continue;
    }

    drawCharToBuffer(cx, cy, *text, color, bg, size);
    cx += 6 * size;

    if (cx >= x + maxWidth) {
      cx = x;
      cy += 8 * size;
    }
    text++;
  }
}

void drawFullScreen(const uint16_t *frame) {
  setAddrWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
  dmaPushFrame(frame, (uint32_t)TFT_WIDTH * TFT_HEIGHT);
}

void fillScreen(uint16_t color) {
  setAddrWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);
  spiSet16Bit();
  gpio_put(TFT_DC, 1);
  gpio_put(TFT_CS, 0);

  // Use DMA to push the same color repeatedly
  dma_channel_config c = dma_channel_get_default_config(dma_chan);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
  channel_config_set_read_increment(&c, false); // Repeat the same value
  channel_config_set_write_increment(&c, false);
  channel_config_set_dreq(&c, spi_get_dreq(spi1, true));
  volatile void *tx_reg = (void*)&(spi_get_hw(spi1) -> dr);

  dma_channel_configure(dma_chan, &c, tx_reg, &color, TFT_WIDTH*TFT_HEIGHT, true);
  dma_channel_wait_for_finish_blocking(dma_chan);
  gpio_put(TFT_CS, 1);
}

void displayTime(
  uint8_t second,
  uint8_t minute,
  uint8_t hour,
  uint8_t day,
  uint8_t month,
  uint8_t year
) {
  char timeStr[16];
  char dateStr[20];

  sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
  sprintf(dateStr, "%02d/%02d/20%02d", day, month, year);

  drawStringToBuffer(80, 40, "Time & Date", 0x07E0, 0x0000, 2);
  drawStringToBuffer(80, 90, timeStr, 0xFFFF, 0x0000, 4);
  drawStringToBuffer(80, 150, dateStr, 0xFFE0, 0x0000, 2);
  pushBuffer();
}

unsigned long previousUpdate = 0;
const unsigned long updateInterval = 1000;

void setup() {
  Serial.begin(115200);
  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();

  pinMode(TFT_BL, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(BUTTON_SELECT, INPUT);
  pinMode(BUTTON_MAIN, INPUT);

  gpio_put(TFT_BL, 1);
  gpio_put(TFT_CS, 1);

  gpio_set_function(TFT_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(TFT_SCK, GPIO_FUNC_SPI);

  spi_init(spi1, 62500000);
  spiSet8Bit();

  // claim a DMA channel
  dma_chan = dma_claim_unused_channel(true);
  if (dma_chan < 0) {
    // fatal - no DMA channel
    while (1) { delay(1000); }
  }

  //setMCP7940DateTime(0, 57, 10, 5, 11, 11, 26);

  initDisplay();
  fillBuffer(0x0000);
  uint8_t sec, min, hour, wday, day, month, year;
  readMCP7940DateTime(sec, min, hour, wday, day, month, year);
  displayTime(sec, min, hour, day, month, year);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousUpdate >= updateInterval) {
    previousUpdate = currentMillis;
    uint8_t sec, min, hour, wday ,day, month, year;
    readMCP7940DateTime(sec, min, hour, wday, day, month, year);
    displayTime(sec, min, hour, day, month, year);
  }
}
