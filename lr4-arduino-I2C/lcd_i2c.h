#ifndef LC_I2C_H__
#define LC_I2C_H__

#include <Wire.h>
#include <Arduino.h>

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