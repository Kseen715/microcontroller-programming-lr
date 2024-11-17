#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // конструктор с указанием адреса расширителя портов 

 byte letter[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000
 };
 
void printString(uint32_t row, uint32_t column, const char *str, const char *new_str){
  char *str_begin = str; //задаю начало строки для обработки

  //выписываю первую строку посимвольно с использованием указателей
  while(*str_begin != '\0'){
    lcd.setCursor(column++, row);
    lcd.print(*str_begin);
    delay(500);
    str_begin++;
  }

  //определяю конец первой строки 
  char *str_end = new_str;
  while(*str_end != '\0'){
    str_end++;
  }

  lcd.setCursor(column++, row);
  lcd.command(0);
  column -= 1;
  delay(200);

  //переписываю строку на новую, начиная с конца
  while(*str_end != *new_str){
    lcd.print(*str_end);
    lcd.setCursor(column--, row);
    delay(500);
    str_end--;
  }

  
  lcd.print(*new_str);
  lcd.setCursor(column, row);

  while(column >= 0){
    lcd.setCursor(column--, row);
    lcd.print(" ");
  }

  while(column >= 0) {
    lcd.setCursor(column--, row);
    lcd.print(" ");
  }

  delay(300);
}

void setup(){
  lcd.init();
  lcd.backlight();

  lcd.command(0x40);
  for (int i = 0; i < 8; i++) {
    lcd.write(letter[i]);
  }

  lcd.write(0);
//  printString(0, 0, "Gulyi", "Igor");
//  lcd.clear();
}

void loop(){
  
}
