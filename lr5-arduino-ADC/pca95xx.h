#ifndef PCA95XX_H_
#define PCA95XX_H_

#include <Arduino.h>
#include <Wire.h>

#define PCA95XX_IN_REG 0x00
#define PCA95XX_OUT_REG 0x01
#define PCA95XX_POL_REG 0x02
#define PCA95XX_CONF_REG 0x03

#define PCA95XX_IN_ALL 0xFF
#define PCA95XX_OUT_ALL 0x00
#define PCA95XX_POL_DEFAULT_ALL 0x00
#define PCA95XX_POL_INVERTED_ALL 0xFF

#define PCA95XX_PIN_RESET_MODE OUTPUT

// Отправляет сигнал RESET для PCA9538.
// Parameters
// ----------
//    resetPin(uint8_t): Вывод сброса
// Returns
// -------
//    NONE
void pca95xx_reset(int8_t resetPin);

// Настраивает все выводы в режим OUT по умполчанию.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
//    polInv(uint8_t): Инверсия выводов (байт настройки)
//    io(uint8_t): Режим ввод/вывод (байт настройки)
// Returns
// -------
//    NONE
void pca95xx_configure(uint8_t addr, uint8_t polInv = PCA95XX_POL_DEFAULT_ALL, uint8_t io = PCA95XX_OUT_ALL);

// Получает данные из PCA95XX.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
// Returns
// -------
//    uint8_t: Данные, полученные от PCA95XX
uint8_t pca95xx_in(uint8_t addr);

// Отправляет данные в PCA95XX.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
//    data(uint8_t): Байт данных
// Returns
// -------
//    NONE
void pca95xx_out(uint8_t addr, uint8_t data);

#include "pca95xx.h"

// Внешний буфер для результатов работы функций
uint8_t pca95xx_i2c_buf = 0u;

// Отправляет байт data с командой cmd в PCA95XX
// с адресом addr.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
//    cmd(uint8_t): Код операции
//    data(uint8_t): Байт данных
// Returns
// -------
//    bool: Возвращает 1 в случае ошибки,
//          0 при удачном выполнении
bool pca95xx_i2c_sendData(uint8_t addr, uint8_t cmd,
                          uint8_t data = 0u)
{
    Wire.beginTransmission(addr);
    Wire.write(cmd);
    if (cmd != PCA95XX_IN_REG)
        Wire.write(data);
    if (Wire.endTransmission())
        return 1;
    return 0;
}

// Получает байт данных из PCA95XX
// с адресом addr.
// Записывает результат в буфер.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
// Returns
// -------
//    bool: Возвращает 1 в случае ошибки,
//          0 при удачном выполнении
bool pca95xx_i2c_receiveData(uint8_t addr)
{
    if (pca95xx_i2c_sendData(addr, PCA95XX_IN_REG))
    {
        return 1;
    }
    delay(15);
    Wire.requestFrom(addr, 1u, 1u);
    while (Wire.available() < 1)
        ;
    pca95xx_i2c_buf = Wire.read();
    return 0;
}

// Отправляет сигнал RESET для PCA9538.
// Parameters
// ----------
//    resetPin(uint8_t): Вывод сброса
// Returns
// -------
//    NONE
void pca95xx_reset(int8_t resetPin)
{
    if (resetPin < 0)
        return;
    pinMode(resetPin, PCA95XX_PIN_RESET_MODE);
    digitalWrite(resetPin, LOW);
    delay(1);
    digitalWrite(resetPin, HIGH);
    delay(1);
}

// Настраивает все выводы в режим OUT по умполчанию.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
//    polInv(uint8_t): Инверсия выводов (байт настройки)
//    io(uint8_t): Режим ввод/вывод (байт настройки)
// Returns
// -------
//    NONE
void pca95xx_configure(uint8_t addr, uint8_t polInv, uint8_t io)
{
    pca95xx_i2c_sendData(addr, PCA95XX_POL_REG, polInv);
    pca95xx_i2c_sendData(addr, PCA95XX_CONF_REG, io);
    delay(15);
}

// Получает данные из PCA95XX.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
// Returns
// -------
//    uint8_t: Данные, полученные от PCA95XX
uint8_t pca95xx_in(uint8_t addr)
{
    if (!pca95xx_i2c_receiveData(addr))
        return 0u;
    return pca95xx_i2c_buf;
}

// Отправляет данные в PCA95XX.
// Parameters
// ----------
//    addr(uint8_t): Адрес микросхемы PCA95XX на I2C
//    data(uint8_t): Байт данных
// Returns
// -------
//    NONE
void pca95xx_out(uint8_t addr, uint8_t data)
{
    pca95xx_i2c_sendData(addr, PCA95XX_OUT_REG, data);
}

#endif // PCA95XX_H_