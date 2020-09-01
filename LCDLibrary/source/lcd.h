#ifndef LCD_H
#define LCD_H

void lcd_init();
void lcd_displayOn();
void lcd_print(char message[]);
void lcd_displayLower(int lowerBits);
void lcd_displayUpper(int upperBits);
void lcd_clear();
void lcd_printInt(int i);
void lcd_printFloat(float f);

#endif
