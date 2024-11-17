#ifndef LC_I2C_H__
#define LC_I2C_H__

#include <Wire.h>

// Основные команды
#define LCD_CLEARDISPLAY 0x01   // Очистить дисплей и установить
                                // курсор в начальное положение
#define LCD_RETURNHOME 0x02     // Установить курсор в начальное
                                // положение
#define LCD_ENTRYMODESET 0x04   // Установать режим ввода данных
#define LCD_DISPLAYCONTROL 0x08 // Режимы дисплея
#define LCD_CURSORSHIFT 0x10    // Сдвиг курсора
#define LCD_FUNCTIONSET 0x20    // Установка размера символа,
                                // кол-во строк дисплея,
                                // инфтерфейс данных
#define LCD_SETCGRAMADDR 0x40   // Установка адреса CGRAM
#define LCD_SETDDRAMADDR 0x80   // Установка адреса DDRAM

// Флаги для всякого разного
#define LCD_ENTRYRIGHT 0x00          // Напр. движ. курсора <--
#define LCD_ENTRYLEFT 0x02           // Напр. движ. курсора -->
#define LCD_ENTRYSHIFTINCREMENT 0x01 // Прокрутка текста <--
                                     // (справа-налево)
#define LCD_ENTRYSHIFTDECREMENT 0x00 // Отключить прокрутку

// Флаги вкл/выкл
#define LCD_DISPLAYON 0x04  // Дисплей ON
#define LCD_DISPLAYOFF 0x00 // Дисплей OFF
#define LCD_CURSORON 0x02   // Курсор ON
#define LCD_CURSOROFF 0x00  // Курсор OFF
#define LCD_BLINKON 0x01    // Мигание ON
#define LCD_BLINKOFF 0x00   // Мигание OFF

// Флаги сдвигов
#define LCD_DISPLAYMOVE 0x08 // Движение всего дисплея,
                             // вместо курсора
#define LCD_CURSORMOVE 0x00  // Движение курсора,
                             // вместо всего дисплея
#define LCD_MOVERIGHT 0x04   // Движение вправо
#define LCD_MOVELEFT 0x00    // Движение влево

// Флаги наборов функций
#define LCD_8BITMODE 0x10 // 8-битный режим
#define LCD_4BITMODE 0x00 // 4-битный режим
#define LCD_2LINE 0x08    // 1 строчка
#define LCD_1LINE 0x00    // 2 строчки
#define LCD_5x10DOTS 0x04 // Режим символов 5x10
#define LCD_5x8DOTS 0x00  // Режим символов 5x8

// Флаги подсветки
#define LCD_BACKLIGHT 0x08   // Подсветка ON
#define LCD_NOBACKLIGHT 0x00 // Подсветка OFF

#define En B00000100 // Бит включения
#define Rw B00000010 // Бит Read/Write
#define Rs B00000001 // Бит выбора регистра

class LC_I2C
{
public:
  // Конструктор
  //
  // Parameters
  // ----------
  //    lcd_addr(uint8_t):	I2C адрес LCD дисплея
  //    lcd_cols(uint8_t):	Кол-во столбцов на дисплее
  //    lcd_rows(uint8_t):	Кол-во строк на дисплее
  //    charsize(uint8_t):	Размер симвлов, LCD_5x10DOTS
  //        или LCD_5x8DOTS
  LC_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows,
         uint8_t charsize = LCD_5x8DOTS);

  // Обязательно вызывать после создания объекта дисплея
  void begin();

  // Очистить дисплей и сбросить положение курсора
  void clear();

  // Возврат курсора
  void home();

  // Выключить вывод символов
  void noDisplay();

  // Показывать символы на дисплее
  void display();

  // Не моргать курсором
  void noBlink();

  // Моргать курсором
  void blink();

  // Выключить курсор
  void noCursor();

  // Включить курсор
  void cursor();

  // Вывод строки
  //
  // Parameters
  // ----------
  //    str(char*): Указатель на начало строки
  void print(char *str);

  // Сдвиг дисплея влево
  void scrollDisplayLeft();

  // Сдвиг дисплея вправо
  void scrollDisplayRight();

  //
  void printLeft();

  //
  void printRight();

  // Порядок текста слева-направо
  void leftToRight();

  // Порядок текста справа-налево
  void rightToLeft();

  // Инкремент сдвига
  void shiftIncrement();

  // Декремент сдвига
  void shiftDecrement();

  // Выключить подсветку
  void noBacklight();

  // Включить подсветку
  void backlight();

  // Возвращает состояние подсветки
  //
  // Returns
  // -------
  //    bool: Состояние подсветки
  bool getBacklight();

  // Выровнять текст по правому краю курсора
  void autoscroll();

  // Выровнять текст по левому краю курсора
  void noAutoscroll();

  // Позволяет заполнить первые 8 байт CGRAM
  // собственными символами
  void createChar(uint8_t, uint8_t[]);

  // Установаить положение курсора
  void setCursor(uint8_t, uint8_t);

  // Написать символ на дисплее
  virtual size_t write(uint8_t);

  // Отправить комманду
  void command(uint8_t);

  // Включить мигание курсора
  inline void blink_on() { blink(); }

  // Выключить мигание курсора
  inline void blink_off() { noBlink(); }

  // Включить курсор
  inline void cursor_on() { cursor(); }

  // Выключить курсор
  inline void cursor_off() { noCursor(); }

  // ---===== Compatibility API function aliases =====---

  // Штука для backlight() и nobacklight()
  void setBacklight(uint8_t new_val);

  // Штука для createChar()
  void load_custom_character(uint8_t char_num, uint8_t *rows);

private:
  // Отправить команду или данные
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);
  uint8_t _addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _charsize;
  uint8_t _backlightval;
};

#endif // LC_I2C_H__

// #include "LC_I2C.h"

LC_I2C::LC_I2C(uint8_t lcd_addr, uint8_t lcd_cols,
               uint8_t lcd_rows, uint8_t charsize)
{
  _addr = lcd_addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _charsize = charsize;
  _backlightval = LCD_BACKLIGHT;
}

void LC_I2C::begin()
{
  Wire.begin();
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  if (_rows > 1)
  {
    _displayfunction |= LCD_2LINE;
  }
  if ((_charsize != 0) && (_rows == 1))
  {
    _displayfunction |= LCD_5x10DOTS;
  }
  delay(50);                    // импульс 50мс
                                // Устанавливаем RS и R/W на 0, чтобы начать команды
  expanderWrite(_backlightval); // выключить подсветку
  delay(1000);
  // Переключаем дисплей в 4-битовый режим
  write4bits(0x03 << 4);
  delayMicroseconds(4500); // ждем 4.5мс
  // Вторая команда
  write4bits(0x03 << 4);
  delayMicroseconds(4500); // ждем 4.5мс
  // Третья команда
  write4bits(0x03 << 4);
  delayMicroseconds(150);
  // Окончательно устанавливаем дисплей в 4-битовый режим
  write4bits(0x02 << 4);
  // Устанавливаем кол-во строк, шрифт и т.д.
  command(LCD_FUNCTIONSET | _displayfunction);
  // Включаем дисплей без курсора и без мигания по умолчанию
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();
  // Очищаем дисплей
  clear();
  // Устанавлием направление текста (для нормальных языков)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // Ждем дальнейших команд
  command(LCD_ENTRYMODESET | _displaymode);
  home();
}

// ---===== Высокоуровневые команды :3 =====---

void LC_I2C::clear()
{
  command(LCD_CLEARDISPLAY);
  delayMicroseconds(2000);
}

void LC_I2C::home()
{
  command(LCD_RETURNHOME);
  delayMicroseconds(2000);
}

void LC_I2C::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row > _rows)
  {
    row = _rows - 1; // считаем с 0
  }
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LC_I2C::noDisplay()
{
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LC_I2C::display()
{
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LC_I2C::noCursor()
{
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LC_I2C::cursor()
{
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LC_I2C::noBlink()
{
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LC_I2C::blink()
{
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LC_I2C::scrollDisplayLeft(void)
{
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LC_I2C::scrollDisplayRight(void)
{
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LC_I2C::leftToRight(void)
{
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LC_I2C::rightToLeft(void)
{
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LC_I2C::autoscroll(void)
{
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LC_I2C::noAutoscroll(void)
{
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

void LC_I2C::createChar(uint8_t location,
                        uint8_t charmap[])
{
  location &= 0x7;
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++)
  {
    write(charmap[i]);
  }
}

void LC_I2C::noBacklight(void)
{
  _backlightval = LCD_NOBACKLIGHT;
  expanderWrite(0);
}

void LC_I2C::backlight(void)
{
  _backlightval = LCD_BACKLIGHT;
  expanderWrite(0);
}

bool LC_I2C::getBacklight()
{
  return _backlightval == LCD_BACKLIGHT;
}

void LC_I2C::print(char *str)
{
  while (*str)
  {
    write((int)*str++);
  }
}

// ---===== Среднеуровневые команды =====---

inline void LC_I2C::command(uint8_t value)
{
  send(value, 0);
}

inline size_t LC_I2C::write(uint8_t value)
{
  send(value, Rs);
  return 1;
}

// ---===== Низкоуровневые команды =====---

void LC_I2C::send(uint8_t value, uint8_t mode)
{
  uint8_t highnib = value & 0xf0;
  uint8_t lownib = (value << 4) & 0xf0;
  write4bits((highnib) | mode);
  write4bits((lownib) | mode);
}

void LC_I2C::write4bits(uint8_t value)
{
  expanderWrite(value);
  pulseEnable(value);
}

void LC_I2C::expanderWrite(uint8_t _data)
{
  Wire.beginTransmission(_addr);
  Wire.write((int)(_data) | _backlightval);
  Wire.endTransmission();
}

void LC_I2C::pulseEnable(uint8_t _data)
{
  expanderWrite(_data | En); // En high
  delayMicroseconds(1);      // импульс >450нс

  expanderWrite(_data & ~En); // En low
  delayMicroseconds(50);      // импульс > 37мкс
}

void LC_I2C::load_custom_character(uint8_t char_num, uint8_t *rows)
{
  createChar(char_num, rows);
}

void LC_I2C::setBacklight(uint8_t new_val)
{
  if (new_val)
  {
    backlight(); // включить подсветку on
  }
  else
  {
    noBacklight(); // выключить подсветку off
  }
}

uint8_t strlen(char *str)
{
  uint8_t size = 0;
  while (*str)
  {
    str++;
    size++;
  }
  return size;
}

// #include "LC_I2C.h"

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
