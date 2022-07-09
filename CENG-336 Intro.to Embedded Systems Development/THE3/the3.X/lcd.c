#include "Includes.h"
#include "lcd.h"
#include <p18cxxx.h>

void Pulse(void){
    LCD_EN = 1;
    __delay_us(LCD_PULSE);
    LCD_EN = 0;
    __delay_us(LCD_PULSE);
}

void InitLCD(void) {
    __delay_ms(20);
    PORTD = 0x30; //00110000
    Pulse();
    
    __delay_ms(6);
    PORTD = 0x30;
    Pulse();
    
    __delay_us(300);
    PORTD = 0x30;
    Pulse();
    
    __delay_ms(2);
    PORTD = 0x20; // 00100000
    Pulse();
        
    LcdCmd(0x2C); // 0010 1100
    LcdCmd(0x0C); // 0000 1100
    LcdClear();
}

void LcdCmd(uint8_t cmd){
    // Send 'cmd' as a command to the LCD.
    
    LCD_RS = 0; // set rs to low
    
    // FIRST 4 BIT
    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD |(cmd & 0xF0);      // Write Upper nibble of data
    
    Pulse();                  // Give pulse on E pin
    
    // SECOND 4 BIT
    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD | ((cmd << 4) & 0xF0);// Write Lower nibble of data

    Pulse();                  // Give pulse on E pin
}
void LcdData(uint8_t data){
    // Send 'data' as data to the LCD.
    
    LCD_RS = 1; // set rs to high
    
    // FIRST 4 BIT
    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD | (data & 0xF0);     // Write Upper nibble of data
    
    Pulse();                  // Give pulse on E pin
    
    // SECOND 4 BIT
    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD | ((data << 4)& 0xF0); // Write Lower nibble of data

    Pulse();                  // Give pulse on E pin
}

void CreateChar(uint8_t addr, uint8_t charmap[]) {
  addr &= 0x7; // we only have 8 locations 0-7
  LcdCmd(0x40 | (addr << 3)); // Set the address for custom char.
  for (int i=0; i<8; i++) {
    LcdData(charmap[i]); // Write char data.
  }
}

void LcdClear(void) {
    LcdCmd(0x01);    // Clear the screen
    __delay_ms(LCD_PULSE);     
}

// this function writes given string to lcd
void LcdStr(char* buf){
    while(*buf){
        LcdData(*buf++);   // print first character on LCD
    }   
}