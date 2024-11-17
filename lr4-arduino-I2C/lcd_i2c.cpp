#include "lcd_i2c.h"

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