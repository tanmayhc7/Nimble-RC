#include "LCD_driver.h"
#include "delay.h"
#include "i2c.h"
#include "string.h"

#define LCD_ID 0x4e // New device ID(New LCD Poggers)
/*Command shortcuts*/
#define LINE1 0x80 // Starting address for screen ROW 0, Column 0
#define LINE2 0xC0 // Starting address for screen ROW 1, Column 0
#define LINE3 0x94 // Starting address for screen ROW 1, Column 0
#define LINE4 0xD4 // Starting address for screen ROW 1, Column 0

#define FOUR_BIT_MODE 0x28
#define DISPLAY_AND_CURSOR 0x0C

#define CLEAR_SCREEN 0x01 // Clear contents on screen and reset cursor to ROW0, Column 0.
#define AUTOINCREMENT_CURSOR 0x06

/*Define Control bit registers*/
#define RS 0x01 // D0 - Read(0)/Write(1) mode to internal DDRAM
#define RW 0x02 // D1 - Read Busy Flag
#define E 0x04  // D2 - Enable for pulsing
#define BL 0x08 // D3 - Backlight of LCD (1)ON (0) OFF

static const i2c_e LCD_I2C_bus = I2C__2;

void LCD__init(void) {
  /*Set function to 4-bit interfacing mode*/
  LCD__command_to_LCD(FOUR_BIT_MODE); // 0x28 Function set for 4-bit mode!

  /*DISPLAY AND CURSOR(0x0C)*/
  LCD__command_to_LCD(DISPLAY_AND_CURSOR); // 0x0C Display On, Cursor blink Off

  /*Display Clear(0x01)*/
  LCD__command_to_LCD(CLEAR_SCREEN); // 0x01 Clear screen and cursor

  /*Entry Mode(0x06)*/
  LCD__command_to_LCD(AUTOINCREMENT_CURSOR); // 0x06 Entry mode, auto increment cursor

  /*Cursor Reset(0x80)*/
  LCD__command_to_LCD(LINE1); // Set DDRAM address to ROW1(start)

  delay__ms(5);
}

void LCD__reset(void) {
  uint8_t Reg = 0x00;
  /*Initiate Reset Sequence for 4-bit mode interfacing*/
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x2C); // 0x20
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x28);

  delay__ms(10);
}

void LCD__command_to_LCD(uint8_t command) {
  uint8_t byte, Reg = 0x00;
  /*Start with the upper nibble*/
  byte = (command & 0xF0);
  byte |= BL;
  byte |= E;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~E;
  byte |= BL;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  /*Now do the lower nibble*/
  byte = (command & 0x0F) << 4;
  byte |= BL;
  byte |= E;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~E;
  byte |= BL;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  // Delay to process commands
  delay__ms(3);
}

void LCD__char_to_LCD(uint8_t character) {
  uint8_t byte, Reg = 0x00;
  /*Start with the upper nibble*/
  byte = (character & 0xF0);
  byte |= BL;
  byte |= (E | RS);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~(E | RS);
  byte |= BL;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  delay__ms(3);
  /*Now do the lower nibble*/
  byte = (character & 0x0F) << 4;
  byte |= BL;
  byte |= (E | RS);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~(E | RS);
  byte |= BL;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  delay__ms(3);
}

void LCD__string_to_LCD(char *input_string) {
  uint8_t i = 0;
  while (i != strnlen(input_string, 16)) {
    LCD__char_to_LCD(input_string[i]);
    i++;
  }
}