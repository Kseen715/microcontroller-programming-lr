#include <Arduino.h>

#include "pca95xx.h"
#include "lcd_i2c.h"

#define joyX A1
#define joyY A2
#define button A3
#define potenzia A0

#define RGB_RED_UP 0b00000111     // BGR
#define RGB_RED_BOTTOM 0b00100011 // BGR

#define RGB_GREEN_UP 0b00001011
#define RGB_GREEN_BOTTOM 0b01000011

#define RGB_YELLOW_UP 0b00001111
#define RGB_YELLOW_BOTTOM 0b01100011

#define RGB_OFF 0b00000011

#define DEBUG

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
     0b00010001,
     0b00010011,
     0b00010101,
     0b00010101,
     0b00011001,
     0b00000000} // и
};

LC_I2C lcd(0x27, 16, 2);

// int button = 2;
int buttonState = 0;
int buttonState1 = 0;

void setup()
{
    lcd.begin();
    lcd.backlight();

    lcd.home();
    lcd.clear();

    pca95xx_configure(0x70);

    lcd.createChar(0, cChars[0]); // К
    lcd.createChar(1, cChars[1]); // в
    lcd.createChar(2, cChars[2]); // н
    lcd.createChar(3, cChars[3]); // Б
    lcd.createChar(4, cChars[4]); // Д
    lcd.createChar(5, cChars[5]); // и

    pinMode(button, INPUT);
    digitalWrite(button, HIGH);
    Serial.begin(9600);
}

unsigned long timeUP = 0;
unsigned long timeDOWN = 0;
unsigned long timeLEFT = 0;
unsigned long timeRIGHT = 0;

void loop()
{
    int xValue = analogRead(joyX);
    int yValue = analogRead(joyY);

    buttonState = digitalRead(button);

#undef DEBUG

    lcd.clear();
#ifdef DEBUG
    lcd.setCursor(0, 0);
    lcd.print("X:");
    lcd.print(String(xValue));
    lcd.print(" Y:");
    lcd.print(String(yValue));

    if (buttonState != buttonState1)
    {
        if (buttonState == LOW)
        {
            lcd.setCursor(0, 1);
            lcd.print("Button pressed");
            delay(1000);
            lcd.setCursor(0, 1);
            lcd.print("               ");
        }
        else
        {
            lcd.setCursor(0, 1);
            lcd.print("Button released");
            delay(1000);
            lcd.setCursor(0, 1);
            lcd.print("                ");
        }
    }
    buttonState1 = buttonState;
#else
    lcd.setCursor(0, 0);
    lcd.write((uint8_t)0);
    lcd.print("ope");
    lcd.write((uint8_t)2);
    lcd.print("e");
    lcd.write((uint8_t)1);
    lcd.print(" ");
    lcd.write((uint8_t)4);
    lcd.print("e");
    lcd.write((uint8_t)2);
    lcd.write((uint8_t)5);
    lcd.print("c");

    lcd.setCursor(0, 1);

    unsigned long timeStart = millis();
    delay(250);
    if (yValue < 470)
    {
        // right
        lcd.print("Right:");
        lcd.print(String(timeRIGHT / 1000));
        lcd.print("s");
        timeRIGHT += 2.1 * (millis() - timeStart);
    }
    else if (yValue > 550)
    {
        // left
        // int timeStart = millis();
        lcd.print("Left:");
        lcd.print(String(timeLEFT / 1000));
        lcd.print("s");
        timeLEFT += 2.1 * (millis() - timeStart);
    }
    else if (xValue < 470)
    {
        // up
        lcd.print("Up:");
        lcd.print(String(timeUP / 1000));
        lcd.print("s");
        timeUP += 2.1 * (millis() - timeStart);
    }
    else if (xValue > 550)
    {
        // down
        lcd.print("Down:");
        lcd.print(String(timeDOWN / 1000));
        lcd.print("s");
        timeDOWN += 2.1 * (millis() - timeStart);
    }
    else
    {
        // unpressed
        lcd.print("                ");
        timeUP = 0;
        timeDOWN = 0;
        timeLEFT = 0;
        timeRIGHT = 0;
    }

#endif // DEBUG

    //     При отклонении джойстика вниз зажигать красный светодиод, при отклонении вверх – зеленый, а
    // при отклонении влево/вправо – мигать верхним/нижним желтым. На экране вывести свое имя,
    // фамилию и считать время нахождения джойстика в каждом положении в секундах.

    if (yValue < 470)
    {
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_OFF);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_YELLOW_UP);
        delay(250);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_OFF);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_YELLOW_BOTTOM);
    }
    else if (yValue > 550)
    {
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_OFF);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_YELLOW_UP);
        delay(250);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_OFF);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_YELLOW_BOTTOM);
    }
    else if (xValue < 470)
    {
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_OFF);
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_GREEN_UP | RGB_GREEN_BOTTOM);
    }
    else if (xValue > 550)
    {
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_RED_UP | RGB_RED_BOTTOM);
    }
    else
    {
        pca95xx_out(0x70, pca95xx_in(0x70) | RGB_OFF);
    }

    int potentia = analogRead(potenzia);

    pca95xx_configure(0x73);

    displayPotentia(3);

    delay(250);
}

void displayPotentia(int value)
{
    byte maskUp = 0b11111111;
    byte maskBottom = 0b00000011;
    pca95xx_out(0x73, maskUp);
    pca95xx_out(0x70, maskBottom);

    // pca95xx_out(0x73, pca95xx_in(0x73) & (0 << (8 - (value - 2))));
    // pca95xx_out(0x73, 0b01111111);

    // value 0-9
    // if (value < 2)
    // {
    //     pca95xx_out(0x70, (1 << value));
    // }
    // else
    // {
    //     pca95xx_out(0x73, (0 << 8 - (value - 2)));
    // }
}
