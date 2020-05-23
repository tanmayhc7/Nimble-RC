#include "unity.h"

#include "Mockdelay.h"
#include "Mocki2c.h"

#include "LCD_driver.h"

static const uint8_t lcd_id = 0x4E;

void test_LCD__init(void) {
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x2C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x28, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x8C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x88, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0xCC, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0xC8, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x1C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x18, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x6C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x68, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x8C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x88, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  delay__ms_Expect(3);

  delay__ms_Expect(5);

  LCD__init();
}

void test_LCD_command(void) {
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x8C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x88, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  delay__ms_Expect(3);
  LCD__command_to_LCD(0x80);
}

void test_LCD_reset(void) {
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x2C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x28, true);

  delay__ms_Expect(10);

  LCD__reset();
}

void test_LCD__char_to_LCD(void) {
  uint8_t test_char = 'A';
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x1D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x18, true);
  delay__ms_Expect(3);

  LCD__char_to_LCD(test_char);
}

void test_LCD__string_to_LCD(void) {
  char test_char[] = "Cat";
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x6D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x68, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x1D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x18, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x7D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x78, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  LCD__string_to_LCD(test_char);
}