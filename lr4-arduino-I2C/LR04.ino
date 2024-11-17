#include "lcd_i2c.h"

LC_I2C lcd(0x27, 16, 2);

byte cChars[][8] = {
    {0b00010001,
     0b00010010,
     0b00010010,
     0b00011100,
     0b00010010,
     0b00010010,
     0b00010001,
     0b00000000}, // К
    {0b00000000,
     0b00000000,
     0b00011110,
     0b00010001,
     0b00011110,
     0b00010001,
     0b00011110,
     0b00000000}, // в
    {0b00000000,
     0b00000000,
     0b00010001,
     0b00010001,
     0b00011111,
     0b00010001,
     0b00010001,
     0b00000000}, // н
    {0b00011111,
     0b00010000,
     0b00010000,
     0b00011110,
     0b00010001,
     0b00010001,
     0b00011111,
     0b00000000}, // Б
    {0b00000100,
     0b00001010,
     0b00001010,
     0b00001010,
     0b00001010,
     0b00011111,
     0b00010001,
     0b00000000}, // Д
    {0b00000000,
     0b00000000,
     0b00001010,
     0b00011111,
     0b00011111,
     0b00001110,
     0b00000100,
     0b00000000}, // heart_big
    {0b00000000,
     0b00000000,
     0b00000000,
     0b00001010,
     0b00011111,
     0b00001110,
     0b00000100,
     0b00000000}, // heart_small
    {0b00000000,
     0b00000000,
     0b00010001,
     0b00010011,
     0b00010101,
     0b00010101,
     0b00011001,
     0b00000000}, // и
    {0b00001110,
     0b00011111,
     0b00011100,
     0b00011000,
     0b00011100,
     0b00011111,
     0b00001110,
     0b00000000}, // pacman_open
    {0b00001110,
     0b00011111,
     0b00011111,
     0b00011111,
     0b00011111,
     0b00011111,
     0b00001110,
     0b00000000}, // pacman_closed
    {0b00000000,
     0b00000000,
     0b00000110,
     0b00001111,
     0b00001111,
     0b00000110,
     0b00000000,
     0b00000000}, // pacman_dot
};

void setup()
{
  lcd.begin();
  lcd.home();
  lcd.backlight();

  lcd.createChar(0, cChars[5]);
  lcd.createChar(1, cChars[6]);
  lcd.createChar(3, cChars[8]);
  lcd.createChar(4, cChars[9]);
  lcd.createChar(5, cChars[10]);
}

void loop()
{

  int speed_div = 500;
  lcd.home();

  lcd.write(3);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  lcd.write(5);
  // Задержка перед началом сдвига
  delay(speed_div);

  // Цикл для сдвига текста вправо
  for (int positionCounter = 0; positionCounter < 16; positionCounter++)
  {
    // Сдвиг вправо
    lcd.scrollDisplayRight();
    if (positionCounter % 2 == 0)
    {
      lcd.createChar(3, cChars[9]);
    }
    else
    {
      lcd.createChar(3, cChars[8]);
    }
    // Задержка между каждым сдвигом
    delay(speed_div);
  }

  // Очистка дисплея перед следующим циклом
  lcd.clear();
}
